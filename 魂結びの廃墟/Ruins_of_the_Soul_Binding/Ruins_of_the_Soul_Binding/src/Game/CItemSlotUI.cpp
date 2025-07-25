#include "CItemSlotUI.h"
#include "CImage.h"
#include "CText.h"
#include "CInventory.h"
#include "CInput.h"
#include "CItemMenu.h"

#define COUNT_TEXT_POS CVector2(40.0f, 30.0f)
#define SLOT_SIZE 137.0f
#define FONT_SIZE 24

// コンストラクタ
CItemSlotUI::CItemSlotUI(int slotIdx)
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mSlotIndex(slotIdx)
	, mpItemData(nullptr)
	, mpIcon(nullptr)
	, mpCountText(nullptr)
{
	// アイテムのアイコン生成
	mpIcon = new CImage
	(
		"", ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);

	// アイテムの個数のテキスト生成
	mpCountText = new CText
	(
		nullptr, FONT_SIZE,
		CVector2::zero, CVector2::zero,
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

// カーソルが重なった時
void CItemSlotUI::OnPointerEnter(const CVector2& pos)
{
	// インベントリにカーソルが重なったことを伝える
	CInventory::Instance()->EnterItemSlot(mSlotIndex);
}

// カーソルが離れた時
void CItemSlotUI::OnPointerExit(const CVector2& pos)
{
	// インベントリにカーソルが離れたことを伝える
	CInventory::Instance()->ExitItemSlot(mSlotIndex);
}

// クリックされた時
void CItemSlotUI::OnPointerDown(const CVector2& pos)
{
	// 空スロットの場合は掴めない
	if (mpItemData == nullptr) return;

	// インベントリにアイテムスロットが掴まれたことを伝える
	CInventory::Instance()->GrabItemSlot(mSlotIndex);
}

// クリックを離したとき
void CItemSlotUI::OnPointerUp(const CVector2& pos)
{
	// 空スロットの場合は離せない
	if (mpItemData == nullptr) return;

	// インベントリにアイテムスロットが離されたことを伝える
	CInventory::Instance()->ReleaseItemSlot(mSlotIndex);
}

// ドラック&ドロップの移動距離
void CItemSlotUI::OnMove(const CVector2& move)
{
	if (mpItemData == nullptr) return;
#if _DEBUG
	CDebugPrint::Print("Move:%s: %.2f, %.2f\n", mpItemData->name.c_str(), move.X(), move.Y());
#endif

	// アイテムアイコンの移動
	mpIcon->SetPos(mpIcon->GetPos() + move);
	mpCountText->SetPos(mpIcon->GetPos() + COUNT_TEXT_POS);
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
#if _DEBUG
			CDebugPrint::Print("Touch:%s\n", mpItemData->name.c_str());
			CDebugPrint::Print("Touch:%p\n", mpItemData);
#endif

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
#if _DEBUG
			CDebugPrint::Print("Enter:%s\n", mpItemData->name.c_str());
			CDebugPrint::Print("Enter:%p\n", mpItemData);
#endif
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
