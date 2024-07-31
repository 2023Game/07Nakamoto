#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

//円周率M_PIを有効にする
#define _USE_MATH_DEFINES
//数学関数のインクルード
#include <math.h>

//デフォルトコンストラクタ
CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mTag(ETag::ENULL)
	, mRadius(0)
{
	//コリジョンマネージャに追加
	CCollisionManager::GetInstance()->Add(this);
}

//コンストラクタ
CCollider::CCollider(CCharacter* parent, CMatrix* matrix,
	const CVector& position, float radius) 
	:CCollider() 
{
//タイプの設定
mType = EType::ESPHERE;
//親設定
mpParent = parent;
//親行列設定
mpMatrix = matrix;
//CTransform設定
mPosition = position;	//位置
//半径設定
mRadius = radius;
}

//デストラクタ
CCollider::~CCollider()
{
	//コリジョンリストから削除
	CCollisionManager::GetInstance()->Remove(this);
}

//親ポインタの取得
CCharacter* CCollider::GetParent()
{
	return mpParent;
}
//コライダタイプの取得
CCollider::EType CCollider::GetType()
{
	return mType;
}

//描画
void CCollider::Render()
{
	glPushMatrix();
	//コライダの中心座標を計算
	//自分の座標×親の変換行列を掛ける
	CVector pos = mPosition * *mpMatrix;
	//中心座標へ移動
	glMultMatrixf(CMatrix().SetTranslate(pos.GetX(), pos.GetY(), pos.GetZ()).GetM());
	//DIFFUSE赤色設定
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

CVector CCollider::GetNormal(CCollider* t)
{
	//三角コライダの法線を求める
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector TYvector = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();

	return TYvector;
}

//衝突判定
bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心ベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.GetLength())
	{
		//衝突している
		return true;
	}
	//衝突していない
	return false;
}

//三角形と球の衝突判定
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();
	//線コライダをワールド座標で作成
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(nullptr, nullptr, sv, ev);
	//三角コライダと線コライダの衝突処理
	return CollisionTriangleLine(t, &line, a);

}

//球と斜面の衝突処理
CVector CCollider::Slope(CCollider* p, CCollider* t, CVector* a)
{
	//三角コライダの法線を求める
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector TYvector = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();

	//printf("法線:%10f %10f %10f\n", TYvector.GetX(), TYvector.GetY(), TYvector.GetZ());
	
	//プレイヤーのZ軸方向のベクトルを求める
	CVector PZvector = CVector(0, 0, 1) * p->mpParent->GetMatrixRotate();
	//プレイヤーのX軸方向のベクトルを求める
	CVector PXvector = TYvector.Cross(PZvector).Nomalize();

	//プレイヤーのZ軸方向のベクトルを求める
	CVector TZvector = PXvector.Cross(TYvector).Nomalize();
	
	double rx, ry, rz;	//求めたラジアンを格納
	float x, y, z;		//求めた度数を格納

	//X軸の回転値(ラジアン)を求める
	rx = -asin(TZvector.GetY());
	//Y軸の回転値(ラジアン)を求める
	ry = atan2(TZvector.GetX(), TZvector.GetZ());
	//Z軸の回転値(ラジアン)を求める
	rz = atan2(PXvector.GetY(), TYvector.GetY());

	//ラジアンから角度を求める
	x = rx * 180 / M_PI;
	y = ry * 180 / M_PI;
	z = rz * 180 / M_PI;
	
	//坂の角度
	*a = CVector(x, y, z);

	return *a;

	/*
	printf("%10f\n", a->GetX());
	printf("%10f\n", ajust.GetX());
	*/

	//確認用
	//printf("ラジアン : %10f %10f %10f\n", rx, ry, rz);
	//printf("度数     : %10f %10f %10f\n", x, y, z);
}


//三角形と線分の衝突判定
bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();
	//三角形の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差していない
	if (dots * dote >= 0.0f)
	{
		//衝突してない(調整不要)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//線分は面と交差している

	//面と線分の好転を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1,頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線とのない席がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算(衝突しない位置まで戻す)
	if (dots < 0.0f)
	{
		//始点が裏面
		*a = normal * -dots;
	}
	else
	{
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}

//優先度の変更
void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::GetInstance()->Remove(this);	//一旦削除
	CCollisionManager::GetInstance()->Add(this);	//追加
}

//優先度の変更
void CCollider::ChangePriority()
{
	//自分の座標×親の変換行列を掛けてワールド座標を求める
	CVector pos = mPosition * *mpMatrix;
	//ベクトルの長さが優先度
	CCollider::ChangePriority(pos.GetLength());
}

//タグの設定
void CCollider::SetTag(ETag tag)
{
	mTag = tag;
}

//タグの取得
CCollider::ETag CCollider::GetTag()
{
	return mTag;
}
