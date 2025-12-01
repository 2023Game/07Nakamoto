#include "CElementSlotUI.h"
#include "CElementManager.h"
#include "CImage.h"

// コンストラクタ
CElementSlotUI::CElementSlotUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpElementSlot(nullptr)
	, mpElementImage(nullptr)
{
	// 装備アイテムのスロットの生成
	mpElementSlot = new CImage
	(
		"UI\\element_frame.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	); 

	// 装備しているアイテムのイメージ読み込み
	mpElementImage = new CImage
	(
		"",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
}

// デストラクタ
CElementSlotUI::~CElementSlotUI()
{
	SAFE_DELETE(mpElementSlot);
	SAFE_DELETE(mpElementImage);
}

// 指定した属性を属性スロットに装備
void CElementSlotUI::EquipElement(const CrystalData* data)
{
	// アイテムのデータが設定されたら
	if (data != nullptr)
	{
		// 設定されたアイテムのアイコンを読み込んで表示
		mpElementImage->Load(data->iconPath.c_str());
		mpElementImage->SetShow(true);
	}
	else
	{
		// アイテムのデータが空だったら
		mpElementImage->Load("");
		mpElementImage->SetShow(false);
	}
}

// 更新
void CElementSlotUI::Update()
{
}

// 描画
void CElementSlotUI::Render()
{
	// 属性スロットの枠を描画
	mpElementSlot->SetAlpha(GetAlpha());
	mpElementSlot->SetPos(mPosition);
	mpElementSlot->Render();

	// 装備されていたら、装備アイコンのアイテムを描画
	if (mpElementImage->IsShow())
	{
		mpElementImage->SetAlpha(GetAlpha());
		mpElementImage->SetPos(mPosition);
		mpElementImage->Render();
	}
}
