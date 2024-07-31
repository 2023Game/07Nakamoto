#include "CMoveFloor.h"
#include "CCollisionManager.h"

//static変数の定義
CModel CMoveFloor::mModelFloor;

//デフォルトコンストラクタ
CMoveFloor::CMoveFloor()
	: mMove(0)
	, mMoveDistance(0)
	, mCLine(this,&mMatrix, CVector(0.0f, 0.0f, 7.0f), CVector(0.0f, 0.0f, -7.0f))
{
	
}

//動く床の設定
void CMoveFloor::SetMoveFloor(const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	mpModel = &mModelFloor;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//行列の更新
	CTransform::Update();
	//コライダーの生成
	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//動く床のモデルの取得
CModel* CMoveFloor::GetModelFloor()
{
	return &mModelFloor;
}

void CMoveFloor::Update()
{
	mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f);

	//行列の更新
	CTransform::Update();
}

void CMoveFloor::Collision(CCollider* m, CCollider* o)
{
	
}

void CMoveFloor::Collision()
{
	//コライダの優先度の変更
	mCLine.ChangePriority();

	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mCLine, COLLISIONRANGE);
}