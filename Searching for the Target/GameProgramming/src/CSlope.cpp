#include "CSlope.h"

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

	mColliderSlope.ColliderMeshSet(this, &mMatrix, mpModel, CCollider::ETag::ESLOPE);
}

//更新処理
void CSlope::Update()
{
	//変換行列の更新
	CTransform::Update();
}