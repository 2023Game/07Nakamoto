#include "CMoveWall.h"
#include "CCollisionManager.h"

//static変数の定義
CModel CMoveWall::mModelWall;

CMoveWall::CMoveWall(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(0)
	, mMoveDistance(0)
	, mpSwhitch(nullptr)
{
	mpSwhitch = parent;
	mpModel = &mModelWall;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 10;

	//行列の更新
	CTransform::Update();

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//更新処理
void CMoveWall::Update()
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

//動く壁のモデルの取得
CModel* CMoveWall::GetModelWall()
{
	return &mModelWall;
}