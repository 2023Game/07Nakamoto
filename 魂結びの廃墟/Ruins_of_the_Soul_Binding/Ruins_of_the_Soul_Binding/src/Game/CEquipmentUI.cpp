#include "CEquipmentUI.h"
#include "CImage.h"

// �R���X�g���N�^
CEquipmentUI::CEquipmentUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpEquipmentSlot(nullptr)
{
	mpEquipmentSlot = new CImage
	(
		"UI\\EquipmentSlot.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpEquipmentSlot->SetCenter(mpEquipmentSlot->GetSize() * 0.5f);

}

// �f�X�g���N�^
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot);
}

// �X�V
void CEquipmentUI::Update()
{
}

// �`��
void CEquipmentUI::Render()
{
	mpEquipmentSlot->SetAlpha(GetAlpha());
	mpEquipmentSlot->SetPos(mPosition);
	mpEquipmentSlot->Render();
}
