#include "CFloor.h"

//コンストラクタ
CFloor::CFloor(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//変換行列の更新
	CTransform::Update();
	
	mCFloor.ColliderMeshSet(this, &mMatrix, CFloor::GetModelFloor() , CCollider::ETag::ESLOPE);
}

//static変数の定義
CModel CFloor::mModelFloor;
//床のモデルを取得する
CModel* CFloor::GetModelFloor()
{
	return &mModelFloor;
}