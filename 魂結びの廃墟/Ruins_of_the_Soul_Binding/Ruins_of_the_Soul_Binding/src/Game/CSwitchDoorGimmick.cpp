#include "CSwitchDoorGimmick.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CSwitchDoor.h"
#include "CSwitch.h"

// �h�A�ɔz�u����m�[�h�̋���
#define DOOR_NODE_DIST 10.0f

// �R���X�g���N�^
CSwitchDoorGimmick::CSwitchDoorGimmick(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mpSwitchDoor(nullptr)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
{
	// �ʒu���]�l�𔽉f
	Position(pos);
	Rotate(angle);

	mpSwitchDoor = new CSwitchDoor(pos, angle, openPos, modelName, colName);
	mpSwitchDoor->SetOwner(this);
	mpSwitchDoor->SetOnChangeFunc(std::bind(&CSwitchDoorGimmick::OnChangeDoor, this));

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode1 = new CNavNode(pos + VectorZ() * DOOR_NODE_DIST);
	mpNavNode2 = new CNavNode(pos - VectorZ() * DOOR_NODE_DIST);

	// �e�����̃h�A�̃m�[�h���m�������I�Ɍq��
	mpNavNode1->AddForcedConnectNode(mpNavNode2);

	// �����_�ł̃h�A�̊J��Ԃ��o�H�T���m�[�h�ɔ��f
	OnChangeDoor();
}

// �f�X�g���N�^
CSwitchDoorGimmick::~CSwitchDoorGimmick()
{
	if (mpSwitchDoor != nullptr)
	{
		mpSwitchDoor->SetOwner(nullptr);
		mpSwitchDoor->Kill();
	}

	// �o�H�T���m�[�h��S�č폜
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode1);
		SAFE_DELETE(mpNavNode2);
	}
}

// �ڑ�����X�C�b�`��ǉ�
void CSwitchDoorGimmick::AddSwitch(CSwitch* sw)
{
	mpSwitchDoor->AddSwitch(sw);
}

//�o�H�T���m�[�h�P�ڂ��J��Ԃ��؂�ւ�������ɌĂяo������
void CSwitchDoorGimmick::OnChangeDoor()
{
	// �J���Ă��邩�ǂ��������߂�
	bool opened = mpSwitchDoor == nullptr || mpSwitchDoor->IsOpened();

	// �J���˂̌o�H���q�����Ă��邩�ݒ�
	mpNavNode1->SetEnableConnect(mpNavNode2, opened);
	mpNavNode2->SetEnableConnect(mpNavNode1, opened);
}