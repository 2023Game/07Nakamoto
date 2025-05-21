#include "CRDoor.h"

// コンストラクタ
CRDoor::CRDoor()
	:CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpR_Door = CResourceManager::Get<CModel>("RightDoor");
	mpR_DoorCol = CResourceManager::Get<CModel>("RightDoorCol");

	mpR_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpR_DoorCol, true);

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

// 更新処理
void CRDoor::Update()
{
}

// 描画処理
void CRDoor::Render()
{
	mpR_Door->Render(Matrix());
}
