#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"

#define COUNT_TEXT_POS CVector2(15.0f, 0.0f)

// コンストラクタ
CItemSlotUI::CItemSlotUI()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mpIcon(nullptr)
	, mpCountText(nullptr)
{
	mpIcon = new CImage
	(
		"", ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);

	mpCountText = new CText
	(
		nullptr, 24,
		CVector2::zero, CVector2(400.0f, 300.0f),
		CColor::black,
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpCountText->SetEnableOutline(true);
	mpCountText->SetOutlineColor(CColor::white);

}

// デストラクタ
CItemSlotUI::~CItemSlotUI()
{
	SAFE_DELETE(mpIcon);
	SAFE_DELETE(mpCountText);
}

// アイテムスロットの設定
void CItemSlotUI::SetItemSloto(const ItemData* data, int count)
{
	if (data == nullptr) return;
	mpIcon->Load(data->iconPath.c_str());
	mpIcon->SetCenter(mpIcon->GetSize() * 0.5f);

	// 同じ位置に複数入れれる場合は
	if (data->slotLimit > 1)
	{
		// 個数のテキストを設定する
		mpCountText->SetText("%d", count);
	}
}

// 更新
void CItemSlotUI::Update()
{
	CUIBase::Update();

	mpIcon->SetPos(mPosition);
	mpCountText->SetPos(mPosition + COUNT_TEXT_POS);

	mpIcon->Update();
	mpCountText->Update();
}

// 描画
void CItemSlotUI::Render()
{
	mpIcon->Render();
	mpCountText->Render();
}
