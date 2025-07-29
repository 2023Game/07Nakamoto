#include "CEquipmentUI.h"
#include "CImage.h"
#include "CInventory.h"

// �R���X�g���N�^
CEquipmentUI::CEquipmentUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpEquipmentSlot1(nullptr)
{
	// �����A�C�e���̃X���b�g�̐���
	mpEquipmentSlot1 = new CImage
	(
		"UI\\EquipmentSlot2.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpEquipmentSlot1->SetCenter(mpEquipmentSlot1->GetSize() * 0.5f);

}

// �f�X�g���N�^
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot1);
}

// �X�V
void CEquipmentUI::Update()
{
}

// �`��
void CEquipmentUI::Render()
{
	mpEquipmentSlot1->SetAlpha(GetAlpha());
	mpEquipmentSlot1->SetPos(mPosition);
	mpEquipmentSlot1->Render();
}
