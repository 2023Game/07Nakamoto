#include "CMoveFloor.h"
#include "CCollisionManager.h"

#define MOVE CVector(0.0f, 0.0f, 0.1f)

//static変数の定義
CModel CMoveFloor::mModelFloor;

//動く床の設定
CMoveFloor::CMoveFloor(const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(MOVE)
	, mTime(0)
	, mFlag(false)
	, mCLine(this, &mMatrix, CVector(0.0f, 0.0f, 5.0f), CVector(0.0f, 0.0f, -5.0f))
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
	if (mFlag)
	{
		if (mTime > 0)
		{
			mTime--;
		}
		else if (mTime == 0)
		{
			mFlag = false;
		}
		
	}
	else if (!mFlag)
	{
		if (mTime <= 0)
			mTime = 120;

		mPosition = mPosition + mMove;

		//行列の更新
		CTransform::Update();
	}
}

void CMoveFloor::Collision(CCollider* m, CCollider* o)
{
	//自身のコライダタイプの判定
	switch (m->GetType())
	{
	case CCollider::EType::ELINE: //線分コライダ
		//相手のコライダが三角コライダの時
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置の更新
				mPosition = mPosition + adjust;
				//移動方向を反転
				mMove = mMove * -1;
				mFlag = true;
				//行列の更新
				CTransform::Update();
			}
		}
		break;
	}
}

void CMoveFloor::Collision()
{
	//コライダの優先度の変更
	mCLine.ChangePriority();

	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mCLine, COLLISIONRANGE);
}

//動いてる量を取得
CVector CMoveFloor::GetMove()
{
	return mMove;
}