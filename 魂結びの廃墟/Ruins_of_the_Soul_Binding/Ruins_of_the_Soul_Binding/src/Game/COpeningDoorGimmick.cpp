#include "COpeningDoorGimmick.h"
#include "COpeningDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CRoom.h"

// �J���˂̒��S�ʒu����̃I�t�Z�b�g��
#define DOOR_OFFSET_DIST 4.95f
// �J���˂ɔz�u����m�[�h�̋���
#define DOOR_NODE_DIST 14.0f

// �R���X�g���N�^
COpeningDoorGimmick::COpeningDoorGimmick(const CVector& pos, const CVector& angle, CRoom* room)
	: CObjectBase(ETag::eGimmick)
	, mpOpeningDoor(nullptr)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
{
	// �ʒu���]�l�𔽉f
	Position(pos);
	Rotation(angle);

	// �J�������̉�]�����߂�v�Z
	CVector rot = angle + CVector(0.0f, 90.0f, 0.0f);

	// �J�������쐬
	mpOpeningDoor = new COpeningDoor(pos, angle, rot, room);
	mpOpeningDoor->SetOwner(this);
	mpOpeningDoor->SetOnChangeFunc(std::bind(&COpeningDoorGimmick::OnChangeDoor, this));
	mpOpeningDoor->SetRoom(room);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode1 = new CNavNode(pos + VectorZ() * DOOR_NODE_DIST - CVector(0.0f, 0.0f, DOOR_OFFSET_DIST));
	mpNavNode2 = new CNavNode(pos - VectorZ() * DOOR_NODE_DIST - CVector(0.0f, 0.0f, DOOR_OFFSET_DIST));

	// �e�����̃h�A�̃m�[�h���m�������I�Ɍq��
	mpNavNode1->AddForcedConnectNode(mpNavNode2);

	// �����_�ł̃h�A�̊J��Ԃ��o�H�T���m�[�h�ɔ��f
	OnChangeDoor();

}

	// �f�X�g���N�^
COpeningDoorGimmick::~COpeningDoorGimmick()
{
	if (mpOpeningDoor != nullptr)
	{
		mpOpeningDoor->SetOwner(nullptr);
		mpOpeningDoor->Kill();
	}

	// �o�H�T���m�[�h��S�č폜
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode1);
		SAFE_DELETE(mpNavNode2);
	}
}

// �I�u�W�F�N�g�̍폜��`����֐�
void COpeningDoorGimmick::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�ꍇ
	if (obj == mpOpeningDoor)
	{
		mpOpeningDoor = nullptr;
		// �J���˂��Ȃ��Ȃ����̂ŁA�o�H���I���ɂ���
		mpNavNode1->SetEnableConnect(mpNavNode2, true);
		mpNavNode2->SetEnableConnect(mpNavNode1, true);
	}
}

//�J���˂̌o�H�T���m�[�h�P�ڂ��J��Ԃ��؂�ւ�������ɌĂяo������
void COpeningDoorGimmick::OnChangeDoor()
{
	// �J���Ă��邩�ǂ��������߂�
	bool opened = mpOpeningDoor == nullptr || mpOpeningDoor->IsOpened();

	// �J���˂̌o�H���q�����Ă��邩�ݒ�
	mpNavNode1->SetEnableConnect(mpNavNode2, opened);
	mpNavNode2->SetEnableConnect(mpNavNode1, opened);
}
