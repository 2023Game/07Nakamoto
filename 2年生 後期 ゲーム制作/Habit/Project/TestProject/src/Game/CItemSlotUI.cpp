#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"

#define COUNT_TEXT_POS CVector2(15.0f, 0.0f)
#define SLOT_SIZE 60.0f

// コンストラクタ
CItemSlotUI::CItemSlotUI()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mpItemData(nullptr)
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
	mpItemData = data;

	CVector2 size(SLOT_SIZE, SLOT_SIZE);

	if (data != nullptr)
	{
		mpIcon->Load(data->iconPath.c_str());
		mpIcon->SetCenter(mpIcon->GetSize() * 0.5f);

		// 同じ位置に複数入れれる場合は
		if (data->slotLimit > 1)
		{
			// 個数のテキストを設定する
			mpCountText->SetText("%d", count);
		}
	}
	else
	{
		mpIcon->Load("");
		mpCountText->SetText("");
	}

	SetSize(size);
	SetCenter(size * 0.5);
	mpIcon->SetPos(mPosition);
}

void CItemSlotUI::OnPointerEnter(const CVector2& pos)
{
}

void CItemSlotUI::OnPointerExit(const CVector2& pos)
{
}

void CItemSlotUI::OnPointerDown(const CVector2& pos)
{
	
}

void CItemSlotUI::OnPointerUp(const CVector2& pos)
{

}

void CItemSlotUI::OnMove(const CVector2& move)
{
	if (mpItemData == nullptr) return;
	CDebugPrint::Print("Move:%s: %.2f, %.2f\n", mpItemData->name.c_str(), move.X(), move.Y());

	// アイテムアイコンの移動
	mpIcon->SetPos(mpIcon->GetPos() + move);
	mpCountText->SetPos(mpIcon->GetPos() + COUNT_TEXT_POS);
	//printf("%.2f, %.2f\n", mpIcon->GetPos().X(), mpIcon->GetPos().Y());
}

// 更新
void CItemSlotUI::Update()
{
	CUIBase::Update();
	
	mpIcon->Update();
	mpCountText->Update();

	if (mpItemData != nullptr)
	{
		// アイコンの上で長押ししている場合
		if (mIsTouch)
		{
			CDebugPrint::Print("Touch:%s\n", mpItemData->name.c_str());

			if (mpIcon->GetSize() == CVector2(SLOT_SIZE, SLOT_SIZE))
			{
				// アイテムをドラッグ中のアイテムを少し大きくする
				mpIcon->SetSize(mpIcon->GetSize() * 1.1f);
			}
		}
		// クリックを離した場合
		else
		{
			// アイテムを元の位置に戻す
			mpIcon->SetSize(SLOT_SIZE, SLOT_SIZE);
			mpIcon->SetPos(mPosition);
			mpCountText->SetPos(mPosition + COUNT_TEXT_POS);

		}

		// アイテムアイコンの上にカーソルがある場合
		if (mIsEnter)
		{
			CDebugPrint::Print("Enter:%s\n", mpItemData->name.c_str());

		}
	}
}

// 描画
void CItemSlotUI::Render()
{
	if (mpItemData == nullptr) return;
	mpIcon->Render();
	mpCountText->Render();
}
