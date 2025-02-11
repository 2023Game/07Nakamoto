#include "CItemMenu.h"

CItemMenu* CItemMenu::spInstance = nullptr;

// �C���X�^���X���擾
CItemMenu* CItemMenu::Instance()
{
	return spInstance;
}

// �R���X�g���N�g
CItemMenu::CItemMenu()
	: mIsOpened(false)
{
	spInstance = this;

	// ���j���[���X�g���쐬
	mMenuList =
	{
		{"Use" , EContent::eUse},
		{"Back", EContent::eBack},
	};
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
	
}

// �`��
void CItemMenu::Render()
{

}
