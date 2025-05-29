#include "CRDoor.h"

#define HP 15	// 体力

// コンストラクタ
CRDoor::CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	// 扉のモデルデータの取得
	mpR_Door = CResourceManager::Get<CModel>("RightDoor");
	// 扉のコライダーデータの取得
	mpR_DoorCol = CResourceManager::Get<CModel>("RightDoorCol");

	// 扉のコライダー生成
	mpR_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpR_DoorCol, true);
	mpR_DoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy });
	mpR_DoorColliderMesh->SetCollisionLayers
	(
		{ 
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy,
			ELayer::eAttackCol,
		}
	);

	mHp = HP;

	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	mInteractStr = "閉まっている";

}

// デストラクタ
CRDoor::~CRDoor()
{
	if (mpR_DoorColliderMesh != nullptr)
	{
		delete mpR_DoorColliderMesh;
		mpR_DoorColliderMesh = nullptr;
	}
}

// 調べる
void CRDoor::Interact()
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
	SetDebugName("右の扉");
#endif
}

// 更新処理
void CRDoor::Update()
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

	// HPが0になったら
	if (mHp <= 0)
	{
		// 壊れる
		Kill();
	}
}

// 描画処理
void CRDoor::Render()
{
	mpR_Door->Render(Matrix());
}
