#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define INVENTORY_ALPHA 0.75f	// �A���t�@�l

// �R���X�g���N�^
CInventory::CInventory()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI\\menu_back.ping",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);


}

// �f�X�g���N�^
CInventory::~CInventory()
{
}

// �J��
void CInventory::Open()
{
}

// ����
void CInventory::Close()
{
}

// �C���x���g�����J���Ă��邩�ǂ���
bool CInventory::IsOpend() const
{
	return false;
}

// ���߂�
void CInventory::Decide(int select)
{
}

// �X�V
void CInventory::Update()
{
}

// �`��
void CInventory::Render()
{
}
