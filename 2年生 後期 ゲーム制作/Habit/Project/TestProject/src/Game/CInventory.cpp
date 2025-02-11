#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"
#include "CItemSlotUI.h"
#include "CExpandButton.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値

#define INVENTORY_WIDTH 918.5f	// インベントリのX座標
#define INVENTORY_HEIGHT 360.0f	// インベントリのY座標

#define ITEM_WIDTH 750.0f		// アイテムを格納するX座標
#define ITEM_HEIGHT 290.0f		// アイテムを格納するY座標
#define ITEMSLOT_SIZE 60.0f		// アイテムのサイズ

#define SLOT_FRAME 5		// アイテムスロットの枠の幅
#define SLOT_COLUMN 6		// アイテムスロットの列数
#define SLOT_ROW 3			// アイテムスロットの行数
// アイテムスロット数
#define SLOT_COUNT (SLOT_COLUMN * SLOT_ROW)

CInventory* CInventory::spInstance = nullptr;

// インスタンスを取得
CInventory* CInventory::Instance()
{
	return spInstance;
}

// コンストラクタ
CInventory::CInventory()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mItemSlots(SLOT_COUNT)
	, mSlotButtons(SLOT_COLUMN)
	, mIsOpened(false)
	, mEnterSlotIndex(-1)
	, mGrabSlotIndex(-1)
	, mpSlotHighlight(nullptr)
{
	spInstance = this;

	// 各アイテムスロットのアイテムアイコン表示用のイメージを作成
	int slotCount = SLOT_COUNT;
	for (int i = 0; i < slotCount; i++)
	{
		SlotData& slot = mItemSlots[i];
		slot.slotUI = new CItemSlotUI(i);
	}

	// インベントリの背景
	mpBackground = new CImage
	(
		"UI\\menu_back.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);

	// インベントリの枠
	mpInventoryFrame = new CImage
	(
		"UI\\inventory2.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpInventoryFrame->SetCenter(mpInventoryFrame->GetSize() * 0.5f);
	mpInventoryFrame->SetPos(CVector2(INVENTORY_WIDTH, INVENTORY_HEIGHT));

	// 閉じるボタン
	mpBackMenu = new CImage
	(
		"UI\\menu_item.png" ,
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackMenu->SetCenter(mpBackMenu->GetSize() * 0.5f);
	mpBackMenu->SetPos(CVector2(WINDOW_WIDTH * 0.7f, WINDOW_HEIGHT * 0.8f));
	mpBackMenu->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);

	// 選択されているボタンを強調する枠
	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, INVENTORY_ALPHA);

	// カーソルが重なっているアイテムスロットを強調表示する
	mpSlotHighlight = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSlotHighlight->SetSize(ITEMSLOT_SIZE, ITEMSLOT_SIZE);
	mpSlotHighlight->SetCenter(mpSlotHighlight->GetSize() * 0.5f);
	mpSlotHighlight->SetColor(CColor::yellow);
	mpSlotHighlight->SetAlpha(0.5f);
	mpSlotHighlight->SetEnable(false);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CInventory::~CInventory()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpInventoryFrame);
	SAFE_DELETE(mpBackMenu);
	SAFE_DELETE(mpSelectFrame);
	SAFE_DELETE(mpSlotHighlight);

	for (SlotData& slot : mItemSlots)
	{
		SAFE_DELETE(slot.slotUI);
	}
}

// 開く
void CInventory::Open()
{
	if (mIsOpened) return;

	// マウスカーソルを表示
	CInput::ShowCursor(true);

	int size = mItemSlots.size();
	// アイテムスロットの位置の設定
	for (int i = 0; i < size; i++)
	{
		SlotData& slot = mItemSlots[i];

		if (slot.data != nullptr)
		{
			slot.slotUI->SetItemSloto(slot.data, slot.count);
		}
		else
		{
			slot.slotUI->SetItemSloto(nullptr, -1);
		}

		int w = i % SLOT_COLUMN;
		int h = i / SLOT_COLUMN;
		float x = ITEM_WIDTH + SLOT_FRAME * (w + 1) + ITEMSLOT_SIZE * w;
		float y = ITEM_HEIGHT + SLOT_FRAME * (h + 1) + ITEMSLOT_SIZE * h;

		slot.slotUI->SetPos(x, y);
	}

	SetEnable(true);
	SetShow(true);
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);

	mIsOpened = true;
}

// 閉じる
void CInventory::Close()
{
	if (!mIsOpened) return;

	// マウスカーソルを非表示
	CInput::ShowCursor(false);

	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);

	mIsOpened = false;
}

// インベントリが開いているかどうか
bool CInventory::IsOpened() const
{
	return mIsOpened;
}

// 決める
void CInventory::Decide(int select)
{
	//switch (select)
	//{
	//case 0:
	//case 1:
	//	break;
	//case 2:
	//	Close();
	//	break;
	//}
}

// アイテムを追加する
void CInventory::AddItem(ItemType type, int count)
{
	// アイテムデータを取得できなかったら、追加できない
	const ItemData* itemData = nullptr;
	bool success = Item::GetItemData(type, &itemData);
	if (!success) return;

	// 追加するアイテムの残り個数
	int remain = count;

	// ■既にアイテムが入っているスロットにアイテムを追加する
	// アイテムスロットの中身を確認して、
	// 同じ種類のアイテムが入っているアイテムスロットを探す
	for (SlotData& slot : mItemSlots)
	{
		// 空のアイテムスロットであれば、次のアイテムスロットを調べる
		if (slot.data == nullptr) continue;
		// アイテムスロットに入っているアイテムの種類と
		// 追加するアイテムの種類が一致しなければ、次のアイテムスロットを調べる
		if (slot.data->type != type) continue;

		// アイテムの種類が一致するアイテムスロットが見つかった
		
		//　追加後のアイテム個数を算出
		int sum = slot.count + remain;

		// 追加後のアイテム個数が上限を超える場合は、
		if (sum > slot.data->slotLimit)
		{
			// 溢れたアイテム個数を計算
			remain = sum - slot.data->slotLimit;
			// 設定するアイテム個数を上限数に設定
			sum = slot.data->slotLimit;
		}
		// 上限を超えなかった場合は、残り個数を0にする
		else
		{
			remain = 0;
		}
		// 見つかったアイテムスロットのアイテム数を設定
		slot.count = sum;

		// 残り個数が0であれば、処理を終える
		if (remain == 0)
		{
			break;
		}
	}

	// ■空のスロットにアイテムを追加する
	// まだ追加するアイテムが残っていた場合
	if (remain > 0)
	{
		// アイテムスロットのリストの先頭から空のスロットを探して、
		// 見つかった空のアイテムスロットにアイテムを入れる
		for (SlotData& slot : mItemSlots)
		{
			// 既にアイテムが入っているスロットはスルー
			if (slot.data != nullptr) continue;

			// 見つかった空のリストに。追加するアイテムのデータを設定
			slot.data = itemData;

			// 追加するアイテムの個数が上限を超えていたら、次のスロットに残す
			int itemCount = remain;
			if (itemCount > slot.data->slotLimit)
			{
				remain = itemCount - slot.data->slotLimit;
				itemCount = slot.data->slotLimit;
			}
			else
			{
				remain = 0;
			}
			slot.count = itemCount;

			// 追加するアイテムの個数が残ってなければ、追加処理終了
			if (remain == 0)
			{
				break;
			}
		}
	}

	// まだ追加するアイテムが残っていた場合
	// アイテムスロットがいっぱいの場合
	if (remain > 0)
	{
		// TODO：インベントリに追加できなかったアイテムはドロップする
			
		// 一旦削除しておく
		remain = 0;
	}	
}

// カーソルがスロットに重なった
void CInventory::EnterItemSlot(int index)
{
	// カーソルが重なっているアイテムスロットの番号を更新
	mEnterSlotIndex = index;

	mpSlotHighlight->SetPos(mItemSlots[mEnterSlotIndex].slotUI->GetPos());
	mpSlotHighlight->SetEnable(true);
}

// カーソルがアイテムスロットから離れた
void CInventory::ExitItemSlot(int index)
{
	// カーソルが重なっているアイテムスロットの番号と一致すれば、
	// 重なっていない状態へ戻す
	if (mEnterSlotIndex == index)
	{
		mEnterSlotIndex = -1;
		mpSlotHighlight->SetEnable(false);
	}
}

// アイテムスロットを掴んだ
void CInventory::GrabItemSlot(int index)
{
	mGrabSlotIndex = index;
}

// 掴んでいるアイテムスロットを離した
void CInventory::ReleaseItemSlot(int index)
{
	// 離されたアイテムスロットと掴んでいるアイテムスロットが違う場合は、処理しない
	if (mGrabSlotIndex != index) return;
	// 何のアイテムも掴んでなければ、処理しない
	if (mGrabSlotIndex == -1) return;
	// カーソルがどのアイテムスロットにも重なっていない場合は、処理しない
	if (mEnterSlotIndex == -1) return;
	// 掴んでいたアイテムスロットと
	// カーソルが重なっているアイテムスロットが同じであれば、処理しない
	if (mGrabSlotIndex == mEnterSlotIndex) return;

	SlotData& grabData = mItemSlots[mGrabSlotIndex];
	SlotData& enterData = mItemSlots[mEnterSlotIndex];

	// 掴んでいるアイテムのスロットの情報を保持しておく
	const ItemData* tmpData = grabData.data;
	int tmpCount = grabData.count;

	// 掴んでいたアイテムスロットに、
	// カーソルが重なっていたアイテムスロットのデータを入れる
	grabData.data = enterData.data;
	grabData.count = enterData.count;
	grabData.slotUI->SetItemSloto(grabData.data, grabData.count);

	// カーソルが重なっていたアイテムスロットには、
	// 掴んでいたアイテムスロットのデータを入れる
	enterData.data = tmpData;
	enterData.count = tmpCount;
	enterData.slotUI->SetItemSloto(enterData.data, enterData.count);

	// 掴んでいた状態を解除
	mGrabSlotIndex = -1;
}

// 更新
void CInventory::Update()
{
	mpBackground->Update();
	mpInventoryFrame->Update();
	mpBackMenu->Update();
	mpSelectFrame->Update();
	mpSlotHighlight->Update();

	// アイテムスロットのイメージの更新
	for (SlotData& slot : mItemSlots)
	{
		slot.slotUI->Update();
	}
}

// 描画
void CInventory::Render()
{
	mpBackground->Render();
	mpInventoryFrame->Render();
	mpBackMenu->Render();

	mpSelectFrame->SetPos(mpBackMenu->GetPos());
	mpSelectFrame->Render();
	if (mpSlotHighlight->IsEnable())
	{
		mpSlotHighlight->Render();
	}

	// アイテムスロットを描画
	int count = mItemSlots.size();
	for (int i = 0; i < count; i++)
	{
		SlotData & slot = mItemSlots[i];
		// 空のスロットは描画しない
		if (slot.data == nullptr) continue;
		// 現在掴んでいるアイテムスロットはこのタイミングでは描画しない
		if (i == mGrabSlotIndex) continue;

		slot.slotUI->Render();
	}

	// アイテムスロットを掴んでいる場合
	if (mGrabSlotIndex >= 0)
	{
		// 空スロットでなければ、このタイミングで描画
		SlotData& slot = mItemSlots[mGrabSlotIndex];
		if (slot.data != nullptr)
		{
			slot.slotUI->Render();
		}
	}
}
