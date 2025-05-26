#include "CLDoor.h"
#include "Maths.h"
#include "CInput.h"

#define MOVE_POS 9.9f	// 移動距離
#define MOVE_TIME 5.0f	// 移動時間

// コンストラクタ
CLDoor::CLDoor(const CVector& pos, const CVector& rot)
	: mDefaultPos(pos)
	, mOpen(false)
	, mElapsedTime(0.0f)
{
	// 扉のモデルデータの取得
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// 扉のコライダーデータの取得
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// 扉のコライダー生成
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eInteractObj, mpL_DoorCol, true);

	// 位置と向きを設定
	Position(mDefaultPos);
	Rotate(rot);

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
	mOpen = !mOpen;
	mInteractStr = mOpen ? "閉まっている" : "開いている";
}

// 更新処理
void CLDoor::Update()
{
	// 閉まっているとき
	if (!mOpen)
	{
		if (CInput::PushKey('E'))
		{

			float per = mElapsedTime / MOVE_TIME;
			Position(mDefaultPos + CVector(MOVE_POS, 0.0f, 0.0f) * sinf(M_PI * 2.0f * per));

			mElapsedTime += 1.0f / 60.0f;
			if (mElapsedTime >= MOVE_TIME)
			{
				mElapsedTime -= MOVE_TIME;
			}

			//Position(mDefaultPos + CVector(MOVE_POS, 0.0f, 0.0f));
			//mOpen = true;
		}
		
	}
	// 開いているとき
	else
	{
		if (CInput::PushKey('E'))
		{
			Position(mDefaultPos);
			//mOpen = false;
		}

		//float per = mElapsedTime / MOVE_TIME;
		//Position(mDefaultPos + mMoveVec * sinf(M_PI * 2.0f * per));

		//mElapsedTime += 1.0f / 60.0f;
		//if (mElapsedTime >= MOVE_TIME)
		//{
		//	mElapsedTime -= MOVE_TIME;
		//}
	}

}

// 描画処理
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
