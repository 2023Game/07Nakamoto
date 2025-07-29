#include "CEquipmentUI.h"
#include "CImage.h"
#include "CInventory.h"

// コンストラクタ
CEquipmentUI::CEquipmentUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpEquipmentSlot1(nullptr)
{
	// 装備アイテムのスロットの生成
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

// デストラクタ
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot1);
}

// 更新
void CEquipmentUI::Update()
{
}

// 描画
void CEquipmentUI::Render()
{
	mpEquipmentSlot1->SetAlpha(GetAlpha());
	mpEquipmentSlot1->SetPos(mPosition);
	mpEquipmentSlot1->Render();
}
