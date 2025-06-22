#include "CDoorGimmickLR.h"
#include "CDoor.h"
#include "CLDoor.h"
#include "CRDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"

// ���E�̃h�A�̒��S�ʒu����̃I�t�Z�b�g��
#define DOOR_OFFSET_DIST 4.95f
// �h�A�ɔz�u����m�[�h�̋���
#define DOOR_NODE_DIST 10.0f

// �R���X�g���N�^
CDoorGimmickLR::CDoorGimmickLR(const CVector& pos, const CVector& angle)
	: CObjectBase(ETag::eGimmick)
	, mpDoorL(nullptr)
	, mpDoorR(nullptr)
	, mpNavNodeL1(nullptr)
	, mpNavNodeL2(nullptr)
	, mpNavNodeR1(nullptr)
	, mpNavNodeR2(nullptr)
{
	// �ʒu���]�l�𔽉f
	Position(pos);
	Rotation(angle);

	// ���E�̃h�A���܂��Ă���Ƃ��̍��W�����߂�v�Z
	CVector posL = pos - VectorX() * DOOR_OFFSET_DIST;
	CVector posR = pos + VectorX() * DOOR_OFFSET_DIST;

	// �����̃h�A���쐬
	mpDoorL = new CLDoor(posL, angle, posR);
	mpDoorL->SetOwner(this);
	mpDoorL->SetOnChangeFunc(std::bind(&CDoorGimmickLR::OnChangeDoor, this));

	// �E���̃h�A���쐬
	mpDoorR = new CRDoor(posR, angle, posL);
	mpDoorR->SetOwner(this);
	mpDoorR->SetOnChangeFunc(std::bind(&CDoorGimmickLR::OnChangeDoor, this));

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNodeL1 = new CNavNode(posL + VectorZ() * DOOR_NODE_DIST);
	mpNavNodeL2 = new CNavNode(posL - VectorZ() * DOOR_NODE_DIST);
	mpNavNodeR1 = new CNavNode(posR + VectorZ() * DOOR_NODE_DIST);
	mpNavNodeR2 = new CNavNode(posR - VectorZ() * DOOR_NODE_DIST);

	// �e�����̃h�A�̃m�[�h���m�������I�Ɍq��
	mpNavNodeL1->AddForcedConnectNode(mpNavNodeL2);
	mpNavNodeR1->AddForcedConnectNode(mpNavNodeR2);

	// �q����m�[�h��������������ۂɁA�e�m�[�h�̌q���Ȃ��m�[�h��ݒ�
	mpNavNodeL1->AddBlockedNode(mpNavNodeR1);
	mpNavNodeL1->AddBlockedNode(mpNavNodeR2);
	mpNavNodeL2->AddBlockedNode(mpNavNodeR1);
	mpNavNodeL2->AddBlockedNode(mpNavNodeR2);

	// �����_�ł̃h�A�̊J��Ԃ��o�H�T���m�[�h�ɔ��f
	OnChangeDoor();

}

// �f�X�g���N�^
CDoorGimmickLR::~CDoorGimmickLR()
{
	// �����̃h�A���c���Ă�����A��������������ăh�A���폜
	if (mpDoorL != nullptr)
	{
		mpDoorL->SetOwner(nullptr);
		mpDoorL->Kill();
	}

	// �E���̃h�A���c���Ă�����A��������������ăh�A���폜
	if (mpDoorR != nullptr)
	{
		mpDoorR->SetOwner(nullptr);
		mpDoorR->Kill();
	}

	// �o�H�T���m�[�h��S�č폜
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNodeL1);
		SAFE_DELETE(mpNavNodeL2);
		SAFE_DELETE(mpNavNodeR1);
		SAFE_DELETE(mpNavNodeR2);
	}
}

// �I�u�W�F�N�g�̍폜��`����֐�
void CDoorGimmickLR::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g�������̃h�A��������
	if (obj == mpDoorL)
	{
		mpDoorL = nullptr;
		// �h�A���Ȃ��Ȃ����̂ŁA�����̌o�H���I���ɂ���
		mpNavNodeL1->SetEnableConnect(mpNavNodeL2, true);
		mpNavNodeL2->SetEnableConnect(mpNavNodeL1, true);
	}

	// �폜���ꂽ�I�u�W�F�N�g�������̃h�A��������
	if (obj == mpDoorR)
	{
		mpDoorR = nullptr;
		// �h�A���Ȃ��Ȃ����̂ŁA�E���̌o�H���I���ɂ���
		mpNavNodeR1->SetEnableConnect(mpNavNodeR2, true);
		mpNavNodeR2->SetEnableConnect(mpNavNodeR1, true);
	}

}

// ���E�̃h�A�̊J��Ԃ��؂�ւ�������ɌĂяo������
void CDoorGimmickLR::OnChangeDoor()
{
	// ���E�̃h�A���J���Ă��邩�ǂ��������߂�
	bool openedL = mpDoorL == nullptr || mpDoorL->IsOpened();
	bool openedR = mpDoorR == nullptr || mpDoorR->IsOpened();

	// �����̃h�A�̌o�H���q�����Ă��邩�ݒ�
	//�i���h�A���󂢂Ă��� & �E�h�A���܂��Ă���j
	mpNavNodeL1->SetEnableConnect(mpNavNodeL2, openedL && !openedR);
	mpNavNodeL2->SetEnableConnect(mpNavNodeL1, openedL && !openedR);

	// �E���̃h�A�̌o�H���q�����Ă��邩�ݒ�
	//�i�E�h�A���󂢂Ă��� & ���h�A���܂��Ă���j
	mpNavNodeR1->SetEnableConnect(mpNavNodeR2, openedR && !openedL);
	mpNavNodeR2->SetEnableConnect(mpNavNodeR1, openedR && !openedL);

}
