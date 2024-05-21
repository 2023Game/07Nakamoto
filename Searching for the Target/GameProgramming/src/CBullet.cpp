#include "CBullet.h"
#include "CPlayer.h"

//デフォルトコンストラクタ
CBullet::CBullet()
	: mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
{

}

//幅と奥行の
void CBullet::SetSize(float w, float d)
{
	//スケール設定
	mScale = CVector(5.0f, 5.0f, 5.0f);
	//三角形の頂点設定
	mT.SetVertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	mT.SetNormal(CVector(0.0f, 1.0f, 0.0f));
}

//衝突処理
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//コライダのmとoが衝突しているか判定
	if (CCollider::Collision(m, o))
	{
		//衝突しているときは無効にする
		mEnabled = false;
	}
}

//更新
void CBullet::Update()
{
	//生存時間の判定
	if (mLife-- > 0) 
	{
		CTransform::Update();
		//位置更新
		mPosition = mPosition + CVector(-2.0f, 0.0f, 0.0f) * mMatrixRotate;
	}
	else
	{
		//無効にする
		mEnabled = false;
	}
	
}