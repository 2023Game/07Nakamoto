#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"
#include "CInventory.h"
#include "CExpandButton.h"

#define BACK_SIZE 60.0f
#define BACK_ADJUST_POS CVector2(30.0f,0.0f)	//�w�i�̒����ʒu

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
{
	spInstance = this;

	// �����`��̔w�i
	mpBack = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetSize(BACK_SIZE, BACK_SIZE);
	mpBack->SetAlpha(0.8f);

	// �g���{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(0.0f, 0.0f),
		CVector2(30.0f, 30.0f), 
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI\\title_quit0.png", "UI\\title_quit1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	//btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	//btn1->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// �߂�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(0.0f, 0.0f),
		CVector2(30.0f, 30.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn2->LoadButtonImage("UI\\title_quit0.png", "UI\\title_quit1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	//btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn2->SetEnable(false);
	//btn1->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn2);
	SetEnable(false);
}

// �f�X�g���N�^
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	SAFE_DELETE(mpBack);
	SAFE_DELETE(mpText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();

}

// �X�V
void CItemMenu::Update()
{

	mpBack->SetPos(mPosition + BACK_ADJUST_POS);
	mpBack->Update();

	if (!CInventory::Instance()->IsEnable())
	{
		SetEnable(false);
	}

	int size = mButtons.size();
	for (int i = 0;i<size; i++)
	{
		if (IsEnable())
		{
			CButton* btn = mButtons[i];
			btn->SetEnable(true);
			btn->SetPos(mPosition + BACK_ADJUST_POS + CVector2(0.0f, 30.0f * i));
			btn->Update();
		}
	}
}

// �`��
void CItemMenu::Render()
{
	mpBack->Render();
	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// �ҋ@���
void CItemMenu::UpdateIdle()
{
}

// ���j���[���J��
void CItemMenu::UpdateOpen()
{
}

// ���j���[�I��
void CItemMenu::UpdateSelect()
{
}

// ��Ԑ؂�ւ�
void CItemMenu::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
}

// [Use]�N���b�N���̃R�[���o�b�N�֐�
void CItemMenu::OnClickUse()
{
}

// [Back]�N���b�N���̃R�[���o�b�N�֐�
void CItemMenu::OnClickBack()
{
}
