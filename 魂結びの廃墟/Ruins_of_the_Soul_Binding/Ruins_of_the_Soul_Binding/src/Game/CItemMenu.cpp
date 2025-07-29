#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"
#include "CInventory.h"
#include "CButton.h"

#define BACK_ADJUST_POS CVector2(30.0f,-30.0f)	//�w�i�̒����ʒu
#define BUTTON_SIZE CVector2(60.0f,30.0f)		// �{�^���̃T�C�Y
#define BUTTON_POS_Y 30.0f		// �{�^����Y���W

CItemMenu* CItemMenu::spInstance = nullptr;

// �C���X�^���X���擾
CItemMenu* CItemMenu::Instance()
{
	return spInstance;
}

// �R���X�g���N�g
CItemMenu::CItemMenu()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mState(EState::eIdle)
	, mpBack(nullptr)
	, mpText(nullptr)
	, mIsOpened(false)
	, mDecision(false)
	, mSelectIndex(-1)
{
	spInstance = this;

	// �g���{�^���𐶐�
	CButton* btn1 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI\\ItemMenu\\use1.png", "UI\\ItemMenu\\use2.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CItemMenu::OnClickUse, this));
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// �����{�^���𐶐�
	CButton* btn2 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn2->LoadButtonImage("UI\\ItemMenu\\equip1.png", "UI\\ItemMenu\\equip2.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn2->SetOnClickFunc(std::bind(&CItemMenu::OnClickUse, this));
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn2);

	// �߂�{�^���𐶐�
	CButton* btn3 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn3->LoadButtonImage("UI\\ItemMenu\\close1.png", "UI\\ItemMenu\\close2.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn3->SetOnClickFunc(std::bind(&CItemMenu::OnClickClose, this));
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn3);

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �\������
void CItemMenu::Open()
{
	SetEnable(true);
	SetShow(true);
	mSelectIndex = -1;
	mDecision = false;

}

// ����
void CItemMenu::Close()
{
	SetEnable(false);
	SetShow(false);
	mSelectIndex = -1;
	mDecision = false;

}

// �A�C�e�����g����
bool CItemMenu::IsUse() const
{
	return mSelectIndex == 0;
}

bool CItemMenu::IsEquipment() const
{
	return mSelectIndex == 1;
}

// �A�C�e�����j���[����邩
bool CItemMenu::IsClose() const
{
	return mSelectIndex == 2;
}

// �X�V
void CItemMenu::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eSelect:
		UpdateSelect();
		break;
	case EState::eUse:

		break;
	case EState::eBack:

		break;
	}

	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CItemMenu::Render()
{
	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// �ҋ@���
void CItemMenu::UpdateIdle()
{
	// �{�^���̈ʒu�̐ݒ�
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		btn->SetPos(mPosition + BACK_ADJUST_POS + CVector2(0.0f, BUTTON_POS_Y * i));
	}
	//ChangeState(EState::eSelect);
}

// ���j���[�I��
void CItemMenu::UpdateSelect()
{
	ChangeState(EState::eIdle);
}

// ��Ԑ؂�ւ�
void CItemMenu::ChangeState(EState state)
{
	// ������Ԃ̏ꍇ�͐؂�ւ��Ȃ�
	if (state == mState) return;

	// ��Ԃ�ύX���āA��Ԋ֘A�̕ϐ��̏�����
	mState = state;
	mStateStep = 0;
}

// [�g��]�N���b�N���̃R�[���o�b�N�֐�
void CItemMenu::OnClickUse()
{
	if (mDecision) return;

	mSelectIndex = 0;
	mDecision = true;
}

// [����]�N���b�N���̃R�[���o�b�N�֐�
void CItemMenu::OnClickEquipment()
{
	if (mDecision) return;

	mSelectIndex = 1;
	mDecision = true;
}

// [�߂�]�N���b�N���̃R�[���o�b�N�֐�
void CItemMenu::OnClickClose()
{
	if (mDecision) return;

	mSelectIndex = 2;
	mDecision = true;
}
