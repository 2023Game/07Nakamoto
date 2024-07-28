#include "CMovingDoor.h"

//static変数の定義
CModel CMovingDoor::mModelCube;

CMovingDoor::CMovingDoor()
	: mMove(0)
	, mMoveDistance(0)
	, mpSwhitch(nullptr)
{

}

//コンストラクタ
CMovingDoor::CMovingDoor(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(0)
	, mMoveDistance(0)
{
	mpSwhitch = parent;
	mpModel = &mModelCube;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 5;

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

void CMovingDoor::SetMovingDoor(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	mpSwhitch = parent;
	mpModel = &mModelCube;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 10;

	//行列の更新
	CTransform::Update();

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//更新処理
void CMovingDoor::Update()
{
	//弾がSwhichオブジェクトに当たったら
	if (mpSwhitch->GetFlag() == true && mMove <= mMoveDistance) 
	{
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f);
		mMove += 0.1;
	}

	//行列の更新
	CTransform::Update();
}

//モデルの取得
CModel* CMovingDoor::GetModelCube()
{
	return &mModelCube;
}