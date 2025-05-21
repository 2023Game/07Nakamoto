#include "CLDoor.h"

CLDoor::CLDoor()
	:CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpL_DoorCol, true);
}

CLDoor::~CLDoor()
{
	if (mpL_DoorColliderMesh != nullptr)
	{
		delete mpL_DoorColliderMesh;
		mpL_DoorColliderMesh = nullptr;
	}
}

void CLDoor::Update()
{
}

void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
