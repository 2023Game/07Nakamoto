#include "CLDoor.h"

// コンストラクタ
CLDoor::CLDoor(const CVector& pos, const CVector& rot)
{
	// 扉のモデルデータの取得
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// 扉のコライダーデータの取得
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// 扉のコライダー生成
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eInteractObj, mpL_DoorCol, true);

	// 位置と向きを設定
	Position(pos);
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
	if (mOpen)
	{

	}
	else
	{
		
	}
}

// 描画処理
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
