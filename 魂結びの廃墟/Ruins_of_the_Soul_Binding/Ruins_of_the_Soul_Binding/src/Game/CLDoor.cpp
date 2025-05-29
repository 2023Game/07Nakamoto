#include "CLDoor.h"

//#define MOVE_POS 9.9f	// 移動距離
//#define MOVE_TIME 5.0f	// 移動時間
#define HP 15			// 体力

// コンストラクタ
CLDoor::CLDoor(const CVector& pos, const CVector& angle,const CVector& openPos)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mIsBroken(false)
{
	// 扉のモデルデータの取得
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// 扉のコライダーデータの取得
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// 扉のコライダー生成
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpL_DoorCol, true);
	mpL_DoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy});
	mpL_DoorColliderMesh->SetCollisionLayers
	(
		{ 
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy ,
			ELayer::eAttackCol
		}
	);

	mHp = HP;

	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	mInteractStr = "閉まっている";
}

// デストラクタ
CLDoor::~CLDoor()
{
	// コライダーの削除
	if (mpL_DoorColliderMesh != nullptr)
	{
		delete mpL_DoorColliderMesh;
		mpL_DoorColliderMesh = nullptr;
	}
}

// 調べる
void CLDoor::Interact()
{
	// 閉まっている
	if (!mIsOpened)
	{
		mIsOpened = true;
		mIsPlaying = true;
	}
	else if (mIsOpened)
	{
		mIsOpened = false;
		mIsPlaying = true;
	}
	
	mInteractStr = mIsOpened ? "閉まっている" : "開いている";

#if _DEBUG
	SetDebugName("左の扉");
#endif
}

// 更新処理
void CLDoor::Update()
{
	if (mIsPlaying)
	{
		// 扉を開くアニメーション
		if (mIsOpened)
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mClosePos, mOpenPos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPos);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
		// 扉を閉じるアニメーション
		else
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mOpenPos, mClosePos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePos);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	// 開閉中ではない
	else
	{
	}		

	if (mIsBroken)
	{
		Kill();
	}

	// HPが0になったら
	if (mHp <= 0)
	{
		mIsBroken = true;	
	}
}

// 描画処理
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
