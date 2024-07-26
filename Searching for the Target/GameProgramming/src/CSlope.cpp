#include "CSlope.h"
#include "CCollisionManager.h"
#include "CCollider.h"

//デフォルトコンストラクタ
CSlope::CSlope()
{

}

//モデルの設定
void CSlope::SetSlope(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//mColliderSlope.ColliderMeshSet(this, &mMatrix, mpModel);
}

//更新処理
void CSlope::Update()
{
	//変換行列の更新
	CTransform::Update();
}

void CSlope::Collision()
{
	//コライダの優先度の変更
	mColliderSlope.ChangePriority();
	//衝突処理の実行
	CCollisionManager::GetInstance()->Collision(&mColliderSlope, COLLISIONRANGE);
}

CMatrix* CSlope::GetMatrix()
{
	return &mMatrix;
}

