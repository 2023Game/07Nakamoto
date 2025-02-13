#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"

#define BACK_SIZE 60.0f

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
	, mpBack(nullptr)
	, mpText(nullptr)
	, mIsOpened(false)
{
	spInstance = this;

	// ���j���[���X�g���쐬
	mMenuList =
	{
		{"Use" , EContent::eUse},
		{"Back", EContent::eBack},
	};

	// �����`��̔w�i
	mpBack = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetSize(BACK_SIZE, BACK_SIZE);
	mpBack->SetAlpha(0.5f);
	mpBack->SetEnable(false);
}

// �f�X�g���N�^
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// �J��
void CItemMenu::Open()
{
	if (mIsOpened) return;

	mIsOpened = true;
}

// ����
void CItemMenu::Close()
{
	if (!mIsOpened) return;

	mIsOpened = false;
}

// �C���x���g�����J���Ă��邩�ǂ���
bool CItemMenu::IsOpened() const
{
	return mIsOpened;
}

// �X�V
void CItemMenu::Update()
{
	mpBack->Update();
}

// �`��
void CItemMenu::Render()
{
	if (mpBack->IsEnable())
	{
		mpBack->Render();
	}
	
}
