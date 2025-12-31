#include "CElementSlotUI.h"
#include "CElementManager.h"
#include "CImage.h"

// 属性スロットの座標
#define ELEMENT_UI_POS CVector2(40.0f,518.0f)
// 属性アイコンの座標
#define AICON_POS_X 50.0f
#define AICON_POS_Y 531.0f
#define AICON_OFFSET_X 72.0f

CElementSlotUI* CElementSlotUI::spInstance = nullptr;

CElementSlotUI* CElementSlotUI::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CElementSlotUI();
	}
	return spInstance;
}

// コンストラクタ
CElementSlotUI::CElementSlotUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpElementSlot(nullptr)
{
	spInstance = this;

	// 装備アイテムのスロットの生成
	mpElementSlot = new CImage
	(
		"UI\\element_5slot.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	); 
	mpElementSlot->SetPos(ELEMENT_UI_POS);

	float offsetPosX = 0;
	// 属性アイコンのイメージを属性スロット分読み込む
	for (int i = 0; i < MAX_SLOT; i++)
	{
		// 属性アイコンのイメージを読み込み
		CImage* icon = new CImage
		(
			"",
			ETaskPriority::eUI,
			0,
			ETaskPauseType::eGame,
			false,
			false
		);
		
		icon->SetPos(AICON_POS_X + offsetPosX, AICON_POS_Y);
		mpElementImages.push_back(icon);

		offsetPosX += AICON_OFFSET_X;
	}
}

// デストラクタ
CElementSlotUI::~CElementSlotUI()
{
	SAFE_DELETE(mpElementSlot);

	for (CImage* icon : mpElementImages)
	{
		SAFE_DELETE(icon);
	}
}

// 指定したスロットに属性アイコンを設定
void CElementSlotUI::SetElement(int index, const CrystalData* data)
{
	// アイコンのデータが設定されたら
	if (data != nullptr)
	{
		mpElementImages[index]->Load(data->iconPath.c_str());
		mpElementImages[index]->SetShow(true);
	}
	else
	{
		mpElementImages[index]->Load("");
		mpElementImages[index]->SetShow(false);
	}
}

// 指定した属性を属性スロットに装備
void CElementSlotUI::EquipElement(const CrystalData* data)
{
	for (CImage* icon : mpElementImages)
	{
		// アイコンのデータが設定されたら
		if (data != nullptr)
		{
			// 設定されたアイテムのアイコンを読み込んで表示
			icon->Load(data->iconPath.c_str());
			icon->SetShow(true);
		}
		else
		{
			// アイテムのデータが空だったら
			icon->Load("");
			icon->SetShow(false);
		}
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
	//mpElementSlot->SetPos(mPosition);
	mpElementSlot->Render();

	for (CImage* icon : mpElementImages)
	{
		// 装備されていたら、装備アイコンのアイテムを描画
		if (icon->IsShow())
		{
			icon->SetAlpha(GetAlpha());
			//icon->SetPos(mPosition);
			icon->Render();
		}
	}
}
