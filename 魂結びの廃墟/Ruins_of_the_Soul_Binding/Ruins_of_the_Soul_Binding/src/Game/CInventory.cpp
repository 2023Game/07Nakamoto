#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"
#include "CItemSlotUI.h"
#include "CExpandButton.h"
#include "CItemMenu.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値

#define INVENTORY_WIDTH 890.0f	// インベントリのX座標
#define INVENTORY_HEIGHT 360.0f	// インベントリのY座標

#define ITEM_WIDTH 750.0f		// アイテムを格納するX座標
#define ITEM_HEIGHT 220.0f		// アイテムを格納するY座標
#define ITEMSLOT_SIZE 137.0f	// アイテムのサイズ

#define SLOT_FRAME 1.5f		// アイテムスロットの枠の幅
#define SLOT_COLUMN 3		// アイテムスロットの列
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
	: CTask(ETaskPriority::eInventry, 0, ETaskPauseType::eMenu)
	, mItemSlots(SLOT_COUNT)
	, mSlotButtons(SLOT_COLUMN)
	, mIsOpened(false)
	, mEnterSlotIndex(-1)
	, mGrabSlotIndex(-1)
	, mpSlotHighlight(nullptr)
	, mpItemMenu(nullptr)
	, mpMenu(nullptr)
{
	spInstance = this;

	// 各アイテムスロットのアイテムアイコン表示用のイメージを作成
	int slotCount = SLOT_COUNT;
	for (int i = 0; i < slotCount; i++)
	{
		SlotData& slot = mItemSlots[i];
		slot.slotUI = new CItemSlotUI(i);
	}

	mpBlakBg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eInventry, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBlakBg->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	mpBlakBg->SetColor(0.0f, 0.0f, 0.0f, 0.5f);

	// インベントリの背景
	mpBackground = new CImage
	(
		"UI\\menu_back.png",
		ETaskPriority::eInventry, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);

	// インベントリの枠
	mpInventoryFrame = new CImage
	(
		"UI\\StatusScreen\\Item_slot.png",
		ETaskPriority::eInventry, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpInventoryFrame->SetCenter(mpInventoryFrame->GetSize() * 0.5f);
	mpInventoryFrame->SetPos(CVector2(INVENTORY_WIDTH, INVENTORY_HEIGHT));

	// カーソルが重なっているアイテムスロットを強調表示する
	mpSlotHighlight = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eInventry, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSlotHighlight->SetSize(ITEMSLOT_SIZE, ITEMSLOT_SIZE);
	mpSlotHighlight->SetCenter(mpSlotHighlight->GetSize() * 0.5f);
	mpSlotHighlight->SetColor(CColor::yellow);
	mpSlotHighlight->SetAlpha(0.5f);
	mpSlotHighlight->SetEnable(false);

	// 選択したアイテムのメニュー覧
	mpItemMenu = new CItemMenu();

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

	SAFE_DELETE(mpBlakBg);
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpInventoryFrame);
	SAFE_DELETE(mpSlotHighlight);
	SAFE_DELETE(mpItemMenu);

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

// 指定された番号のアイテムスロットを返す
const ItemData* CInventory::GetItemSlotData(int slotIndex) const
{
	// 範囲がいであれば、nullptrを返す
	if (!(0 <= slotIndex && slotIndex < mItemSlots.size())) return nullptr;
	return mItemSlots[slotIndex].data;
}

// 指定したアイテムスロットのアイテムを使用
void CInventory::UseItemSlot(int index)
{
	CPlayer2* player = CPlayer2::Instance();
	SlotData& slot = mItemSlots[index];

	// アイテムを使用
	player->UseItem(slot.data);
	// 使用したアイテムの個数を減らす
	slot.count--;
	// アイテムが無くなれば、アイテムスロットを空にする
	if (slot.count == 0)
	{
		slot.data = nullptr;

		// プレイヤーに装備解除を伝える
		player->EquipItem(-1);
	}
	// アイテムスロットの情報をUIに反映
	slot.slotUI->SetItemSloto(slot.data, slot.count);
}

// カーソルがスロットに重なった
void CInventory::EnterItemSlot(int index)
{
	// カーソルが重なっているアイテムスロットの番号を更新
	mEnterSlotIndex = index;
}

// カーソルがアイテムスロットから離れた
void CInventory::ExitItemSlot(int index)
{
	// カーソルが重なっているアイテムスロットの番号と一致すれば、
	// 重なっていない状態へ戻す
	if (mEnterSlotIndex == index)
	{
		mEnterSlotIndex = -1;
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

	// プレイヤーがアイテムを装備しているか
	CPlayer2* player = CPlayer2::Instance();
	int equipSlotIndex = player->GetEquipItemSlotIndex();
	if (equipSlotIndex >= 0)
	{
		// 掴んで移動したアイテムが装備アイテムであれば、
		// 移動後のスロットのアイテムを装備アイテムとして設定
		if (equipSlotIndex == mGrabSlotIndex) player->EquipItem(mEnterSlotIndex);
		// 離したスロットのアイテムが装備アイテムであれば、
		// 掴んだスロットと入れ替えたアイテムを装備アイテムとして設定
		else if (equipSlotIndex == mEnterSlotIndex) player->EquipItem(mGrabSlotIndex);
	}

	// 掴んでいた状態を解除
	mGrabSlotIndex = -1;
}

// 指定したアイテムスロットのアイテムを装備
void CInventory::EquipItemSlot(int index)
{
	CPlayer2* player = CPlayer2::Instance();
	player->EquipItem(index);
}

// 更新
void CInventory::Update()
{
	// アイテムメニューが表示されていない場合
	if (!mpItemMenu->IsShow())
	{
		mpBackground->Update();
		mpInventoryFrame->Update();

		// アイテムスロットのイメージの更新
		for (SlotData& slot : mItemSlots)
		{
			slot.slotUI->Update();
		}

		if (mEnterSlotIndex != -1)
		{
			SlotData& itemData = mItemSlots[mEnterSlotIndex];

			// アイテムアイコンの上で右クリックしたら
			if (CInput::Key(VK_RBUTTON) && !mpItemMenu->IsShow())
			{
				// アイテムアイコンがある場合
				if (itemData.data != nullptr)
				{
					mpItemMenu->SetPos(itemData.slotUI->GetPos());
					// アイテムメニューを表示する
					mpItemMenu->Open();
					mpItemMenu->Update();
				}
			}
			else
			{
				mpSlotHighlight->SetPos(itemData.slotUI->GetPos());
				mpSlotHighlight->SetEnable(true);
				mpSlotHighlight->Update();
			}
		}
		else if (mEnterSlotIndex == -1)
		{
			mpSlotHighlight->SetEnable(false);
		}
	}
	// アイテムメニューが表示されていなければ
	else
	{
		mpItemMenu->Update();

		// 「使う」を押した場合
		if (mpItemMenu->IsUse())
		{
			CPlayer2* player = CPlayer2::Instance();
			SlotData& slot = mItemSlots[mEnterSlotIndex];

			// アイテムが使用できるかどうか
			if (player->CanUseItem(slot.data))
			{
				// アイテムを使用する前に、アイテムメニューとインベントリを閉じる
				mpItemMenu->Close();
				Close();

				// アイテムを使用
				UseItemSlot(mEnterSlotIndex);
			}
			// アイテムが使用できなかった場合
			else
			{
				// TODO:アイテムが使用できなかった理由を画面に表示
				mpItemMenu->Close();
			}
		}
		// 「装備」を押した場合
		else if (mpItemMenu->IsEquipment())
		{
			// アイテムを装備したら、アイテムメニューとインベントリを閉じる
			mpItemMenu->Close();
			Close();

			EquipItemSlot(mEnterSlotIndex);
		}
		// 「閉じる」を押した場合
		else if (mpItemMenu->IsClose())
		{
			mpItemMenu->Close();
		}
	}
}

// 描画
void CInventory::Render()
{
	mpBlakBg->Render();
	mpBackground->Render();
	mpInventoryFrame->Render();

	if (mpSlotHighlight->IsEnable())
	{
		mpSlotHighlight->Render();
	}

	// アイテムスロットを描画
	int count = mItemSlots.size();
	for (int i = 0; i < count; i++)
	{
		SlotData& slot = mItemSlots[i];
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

	// アイテムメニューが有効なら
	if (mpItemMenu->IsEnable())
	{
		// 描画する
		mpItemMenu->Render();
	}

}

