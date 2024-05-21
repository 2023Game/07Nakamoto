#include "CCollider.h"
#include "CCollisionManager.h"

//デフォルトコンストラクタ
CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
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