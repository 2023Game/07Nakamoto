#include "CEquipmentUI.h"
#include "CImage.h"

// コンストラクタ
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

// デストラクタ
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot);
}

// 更新
void CEquipmentUI::Update()
{
}

// 描画
void CEquipmentUI::Render()
{
	mpEquipmentSlot->SetAlpha(GetAlpha());
	mpEquipmentSlot->SetPos(mPosition);
	mpEquipmentSlot->Render();
}
