#include "CLDoor.h"

// �R���X�g���N�^
CLDoor::CLDoor(const CVector& pos, const CVector& rot)
{
	// ���̃��f���f�[�^�̎擾
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// ���̃R���C�_�[����
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eInteractObj, mpL_DoorCol, true);

	// �ʒu�ƌ�����ݒ�
	Position(pos);
	Rotate(rot);

	mInteractStr = "�܂��Ă���";
}

// �f�X�g���N�^
CLDoor::~CLDoor()
{
	// �R���C�_�[�̍폜
	if (mpL_DoorColliderMesh != nullptr)
	{
		delete mpL_DoorColliderMesh;
		mpL_DoorColliderMesh = nullptr;
	}
}

// ���ׂ�
void CLDoor::Interact()
{
	mOpen = !mOpen;
	mInteractStr = mOpen ? "�܂��Ă���" : "�J���Ă���";
}

// �X�V����
void CLDoor::Update()
{
	if (mOpen)
	{

	}
	else
	{
		
	}
}

// �`�揈��
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
