#include "CRDoor.h"

// �R���X�g���N�^
CRDoor::CRDoor()
	:CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpR_Door = CResourceManager::Get<CModel>("RightDoor");
	mpR_DoorCol = CResourceManager::Get<CModel>("RightDoorCol");

	mpR_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpR_DoorCol, true);

}

// �f�X�g���N�^
CRDoor::~CRDoor()
{
	if (mpR_DoorColliderMesh != nullptr)
	{
		delete mpR_DoorColliderMesh;
		mpR_DoorColliderMesh = nullptr;
	}
}

// �X�V����
void CRDoor::Update()
{
}

// �`�揈��
void CRDoor::Render()
{
	mpR_Door->Render(Matrix());
}
