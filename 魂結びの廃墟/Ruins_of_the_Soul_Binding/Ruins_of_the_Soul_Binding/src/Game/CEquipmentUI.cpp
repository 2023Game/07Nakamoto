#include "CEquipmentUI.h"
#include "CImage.h"
#include "CInventory.h"

// コンストラクタ
CEquipmentUI::CEquipmentUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpEquipmentSlot1(nullptr)
	, mpEquipItemImage(nullptr)
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

	// 装備しているアイテムのイメージ読み込み
	mpEquipItemImage = new CImage
	(
		"",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpEquipItemImage->SetCenter(mpEquipmentSlot1->GetSize() * 0.5f);
}

// デストラクタ
CEquipmentUI::~CEquipmentUI()
{
	SAFE_DELETE(mpEquipmentSlot1);
}

// 指定したアイテムスロットのアイテムを装備
void CEquipmentUI::EquipItem(const ItemData* item)
{
	// アイテムのデータが設定されたら
	if (item != nullptr)
	{
		// 設定されたアイテムのアイコンを読み込んで表示
		mpEquipItemImage->Load(item->iconPath.c_str());
		mpEquipItemImage->SetShow(true);
	}
	else
	{
		// アイテムのデータが空だったら
		mpEquipItemImage->Load("");
		mpEquipItemImage->SetShow(false);
	}
}

// 更新
void CEquipmentUI::Update()
{
}

// 描画
void CEquipmentUI::Render()
{
	// 装備アイテムの枠を描画
	mpEquipmentSlot1->SetAlpha(GetAlpha());
	mpEquipmentSlot1->SetPos(mPosition);
	mpEquipmentSlot1->Render();

	// 装備していたら、装備アイコンのアイテムを描画
	if (mpEquipItemImage->IsShow())
	{
		mpEquipItemImage->SetAlpha(GetAlpha());
		mpEquipItemImage->SetPos(mPosition);
		mpEquipItemImage->Render();
	}
}
