#include "CSlope.h"
#include "CCollisionManager.h"
#include "CCollider.h"
#include "CColliderMesh.h"

//モデルの設定
CSlope::CSlope(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//変換行列の更新
	CTransform::Update();
	//コライダの生成
	mCSlope.ColliderMeshSet(this, &mMatrix, CSlope::GetModelSlope(), CCollider::ETag::ESLOPE);

}

CMatrix* CSlope::GetMatrix()
{
	return &mMatrix;
}

void CSlope::Update()
{
	CTransform::Update();
}

//static変数の定義
CModel CSlope::mModelSlope;

//坂のモデルを取得する
CModel* CSlope::GetModelSlope()
{
	return &mModelSlope;
}