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
void CElementSlotUI::EquipElement()
{
	CElementManager::ElementType typ = CElementManager::Instance()->GetCurrentElement();

	switch (typ)
	{
	// 炎属性
	case CElementManager::ElementType::eFire:
		mpElementImage->Load("UI\\fire_icon.png");
		mpElementImage->SetShow(true);
		break;
	// 水属性
	case CElementManager::ElementType::eWater:
		mpElementImage->Load("");
		mpElementImage->SetShow(true);
		break;
	// 雷属性
	case CElementManager::ElementType::eThunder:
		mpElementImage->Load("");
		mpElementImage->SetShow(true);
		break;
	// 氷属性
	case CElementManager::ElementType::eIce:
		mpElementImage->Load("");
		mpElementImage->SetShow(true);
		break;
	// 風属性
	case CElementManager::ElementType::eWind:
		mpElementImage->Load("");
		mpElementImage->SetShow(true);
		break;
	// 無属性
	default:
		mpElementImage->Load("");
		mpElementImage->SetShow(false);
		break;
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

	// 装備していたら、装備アイコンのアイテムを描画
	//if (mpElementImage->IsShow())
	{
		mpElementImage->SetAlpha(GetAlpha());
		mpElementImage->SetPos(mPosition);
		mpElementImage->Render();
	}
}
