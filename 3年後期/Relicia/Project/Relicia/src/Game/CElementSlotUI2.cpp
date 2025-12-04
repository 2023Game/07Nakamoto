#include "CElementSlotUI2.h"
#include "CElementManager.h"
#include "CImage.h"

// 属性スロット
#define ELEMENT_UI_POS CVector2(40.0f,518.0f)	// 座標
#define ELEMENT_UI_ALPHA 0.8f	// アルファ値

// 属性アイコンの座標
//#define ICON_POS_X 50.0f	// X座標
//#define ICON_POS_Y 531.0f	// Y座標

#define ICON_POS_X 568.0f	// X座標
#define ICON_POS_Y 531.0f	// Y座標
#define ICON_OFFSET_X 72.0f	// オフセットX座標
#define ICON_OFF_SIZE 0.85f	// 装備されていないアイコンのサイズ
#define ICON_BACK_A 0.5f	// アルファ値

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
	mpElementSlot->SetAlpha(ELEMENT_UI_ALPHA);

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
void CElementSlotUI2::RenderIcon(int index, float x, float scale, float buttom)
{
	const CrystalData* data = CElementManager::Instance()->GetCurrentElementData(index);
	if (!data) return;

	mpElementImages[index]->Load(data->iconPath.c_str());
	mpElementImages[index]->SetPos(x, ICON_POS_Y);
	mpElementImages[index]->SetScale(buttom);
	mpElementImages[index]->SetUV(0.0f, 0.0f, 1.0f, buttom);
	mpElementImages[index]->Render();
}

// 属性アイコンの背景を設定
void CElementSlotUI2::RenderBackIcon(int index, float x, float scale)
{
	const CrystalData* data = CElementManager::Instance()->GetCurrentElementData(index);
	if (!data) return;

	mpElementBackImages[index]->Load(data->iconPath.c_str());
	mpElementBackImages[index]->SetPos(x, ICON_POS_Y);
	mpElementBackImages[index]->SetUV(0.0f, 0.0f, 1.0f, 1.0f);
	mpElementBackImages[index]->Render();
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

	// 属性ゲージの背景アイコン画像
	RenderBackIcon(left, ICON_POS_X,					 ICON_OFF_SIZE);
	RenderBackIcon(cur,  ICON_POS_X + ICON_OFFSET_X,	 1.0f);
	RenderBackIcon(right,ICON_POS_X + ICON_OFFSET_X * 2, ICON_OFF_SIZE);

	// 属性ゲージが貯まっている分、属性アイコンの描画
	RenderIcon(left, ICON_POS_X, ICON_OFF_SIZE, mgr->GetEnergy(left) / MAX_ENERGY);
	RenderIcon(cur, ICON_POS_X + ICON_OFFSET_X, 1.0f, mgr->GetEnergy(cur) / MAX_ENERGY);
	RenderIcon(right, ICON_POS_X + ICON_OFFSET_X * 2, ICON_OFF_SIZE, mgr->GetEnergy(right) / MAX_ENERGY);

}
