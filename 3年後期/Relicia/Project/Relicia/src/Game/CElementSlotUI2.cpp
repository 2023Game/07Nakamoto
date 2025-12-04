#include "CElementSlotUI2.h"
#include "CElementManager.h"
#include "CImage.h"

// 属性スロットの座標
#define ELEMENT_UI_POS CVector2(40.0f,518.0f)
// 属性アイコンの座標
#define ICON_POS_X 50.0f
#define ICON_POS_Y 531.0f
#define ICON_OFFSET_X 72.0f

// 属性アイコンの背景のアルファ値
#define ICON_BACK_A 0.7f

// コンストラクタ
CElementSlotUI2::CElementSlotUI2()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpElementSlot(nullptr)
{
	// 属性スロットの枠を生成
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

	CVector2 pos = CVector2(ICON_POS_X, ICON_POS_Y);
	// 属性アイコンのイメージを属性スロット分生成
	for (int i = 0; i < MAX_SLOT; i++)
	{
		// 属性アイコンを生成
		CImage* icon = new CImage
		(
			"",
			ETaskPriority::eUI,
			0,
			ETaskPauseType::eGame,
			false,
			false
		);
		icon->SetPos(pos);
		mpElementImages.push_back(icon);

		icon->SetAlpha(ICON_BACK_A);
		mpElementBackImages.push_back(icon);
		pos = CVector2(pos.X() + ICON_OFFSET_X ,pos.Y());
	}
}

// デストラクタ
CElementSlotUI2::~CElementSlotUI2()
{
	SAFE_DELETE(mpElementSlot);

	for (CImage* icon : mpElementImages)
	{
		SAFE_DELETE(icon);
	}
	mpElementImages.clear();
}

// 指定したスロットに属性アイコンを設定
void CElementSlotUI2::SetElement(int index, const CrystalData* data)
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

// 属性アイコンの設定
void CElementSlotUI2::RenderIcon(int index, float x, float scale, float alpha)
{
	const CrystalData* data = CElementManager::Instance()->GetCurrentElementData(index);
	if (!data) return;

	mpElementImages[index]->Load(data->iconPath.c_str());
	mpElementImages[index]->SetPos(x, ICON_POS_Y);
	mpElementImages[index]->SetScale(scale);
	mpElementImages[index]->SetAlpha(alpha);
	mpElementImages[index]->Render();
}

// 更新
void CElementSlotUI2::Update()
{
}

// 描画
void CElementSlotUI2::Render()
{
	// 属性スロットの枠を描画
	mpElementSlot->SetAlpha(GetAlpha());
	mpElementSlot->Render();

	CElementManager* mgr = CElementManager::Instance();
	int cur = mgr->GetCurrentIndex();
	int size = MAX_SLOT;

	int left = (cur - 1 + size) % size;
	int right = (cur + 1) % size;

	RenderIcon(left, ICON_POS_X, 0.85f, mgr->GetEnergy(left) / mgr->GetEnergy(left));
	RenderIcon(cur, ICON_POS_X + 72.0f, 1.2f, mgr->mSlots[cur].currentEnergy / mgr->mSlots[cur].maxEnergy);
	RenderIcon(right, ICON_POS_X + 144.0f, 0.85f, mgr->mSlots[right].currentEnergy / mgr->mSlots[right].maxEnergy);
}
}
