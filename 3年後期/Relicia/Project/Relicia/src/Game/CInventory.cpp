#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CAdventurer.h"
#include "CItemSlotUI.h"
#include "CItemMenu.h"
#include "CGameData.h"

// インベントリの座標
#define INVENTORY_POS CVector2(280.0f,220.5f)
#define INVENTORY_SIZE CVector2(360.0f,285.0f)

#define ICON_POS_X 130.0f	// アイテムアイコンの開始座標X
#define ICON_POS_Y 108.0f	// アイテムアイコンの開始座標Y
#define ICON_INTERVAL 15.0f	// アイコン同士の間隔
#define ICON_PITCH ICON_SIZE + ICON_INTERVAL // アイコン同士の間隔と大きさを合わせた値

#define HIGHLIGHT_SIZE 64.0f		// ハイライトのサイズ

CInventory* CInventory::spInstance = nullptr;

// インスタンスを取得
CInventory* CInventory::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CInventory();
	}
	return spInstance;
}

// コンストラクタ
CInventory::CInventory()
	: CTask(ETaskPriority::eInventry, 0, ETaskPauseType::eMenu)
	, mSlotRow(INITIAL_SLOT_ROW)
	, mSlotCol(INITIAL_SLOT_COL)
	, mIsOpened(false)
	, mEnterSlotIndex(-1)
	, mGrabSlotIndex(-1)
	//, mpItemMenu(nullptr)
{
	spInstance = this;

	// 各アイテムスロットのアイテムアイコン表示用のイメージを作成
	for (int i = 0; i < CGameData::ItemSlots.size(); i++)
	{
		SlotData& slot = CGameData::ItemSlots[i];

		CItemSlotUI* slotUI = new CItemSlotUI(i);

		slotUI->SetSize(CVector2(DISP_ICON_SIZE, DISP_ICON_SIZE));
		slotUI->SetCenter(slotUI->GetSize() * 0.5f);

		int col = i % INITIAL_SLOT_COL;
		int row = i / INITIAL_SLOT_COL;

		float x = ICON_POS_X  + (ICON_SIZE + ICON_INTERVAL) * col;
		float y = ICON_POS_Y  + (ICON_SIZE + ICON_INTERVAL) * row;

		slotUI->SetPos(x, y);

		mItemSlotUIs.push_back(slotUI);
	}

	// メニュー画面の背景生成
	mpMenuBg = new CImage
	(
		"UI\\inventry_back.png",
		ETaskPriority::eInventry, 0, ETaskPauseType::eMenu,
		false, false
	);

	// インベントリの枠
	mpInventoryFrame = new CImage
	(
		"UI\\inventory.png",
		ETaskPriority::eInventry, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpInventoryFrame->SetSize(INVENTORY_SIZE);
	mpInventoryFrame->SetCenter(mpInventoryFrame->GetSize() * 0.5f);
	mpInventoryFrame->SetPos(INVENTORY_POS);

	// カーソルが重なっているアイテムスロットを強調表示する
	mpSlotHighlight = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eInventry, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpSlotHighlight->SetSize(CVector2(ICON_SIZE, ICON_SIZE) * 1.1f);
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

	SAFE_DELETE(mpMenuBg);
	SAFE_DELETE(mpInventoryFrame);
	SAFE_DELETE(mpSlotHighlight);

	for (CItemSlotUI* slot : mItemSlotUIs)
	{
		SAFE_DELETE(slot);
	}

}

// 開く
void CInventory::Open()
{
	if (mIsOpened) return;

	// マウスカーソルを表示
	CInput::ShowCursor(true);

	int size = CGameData::ItemSlots.size();
	// アイテムスロットの位置の設定
	for (int i = 0; i < size; i++)
	{
		SlotData& slot = CGameData::ItemSlots[i];
		CItemSlotUI* slotUI = mItemSlotUIs[i];
		if (slot.data != nullptr)
		{
			slotUI->SetItemSloto(slot.data, slot.count);
		}
		else
		{
			slotUI->SetItemSloto(nullptr, -1);
		}
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
void CInventory::AddItem(ItemId type, int count)
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
	for (SlotData& slot : CGameData::ItemSlots)
	{
		// 空のアイテムスロットであれば、次のアイテムスロットを調べる
		if (slot.data == nullptr) continue;
		// アイテムスロットに入っているアイテムの種類と
		// 追加するアイテムの種類が一致しなければ、次のアイテムスロットを調べる
		if (slot.data->id != type) continue;

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
		for (SlotData& slot : CGameData::ItemSlots)
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
	if (!(0 <= slotIndex && slotIndex < CGameData::ItemSlots.size())) return nullptr;
	return CGameData::ItemSlots[slotIndex].data;
}

// インベントリ内の合計売却価格を取得
const int CInventory::GetTotalSellPrice() const
{
	int totalSellPrice = 100;

	// インベントリの中の合計売却金額を集計
	for (SlotData& slot : CGameData::ItemSlots)
	{
		// 空のアイテムスロットであれば、次のアイテムスロットを調べる
		if (slot.data == nullptr) continue;

		totalSellPrice += slot.data->sellPrice * slot.count;
	}

	return totalSellPrice;
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

	SlotData& grabData = CGameData::ItemSlots[mGrabSlotIndex];
	SlotData& enterData = CGameData::ItemSlots[mEnterSlotIndex];
	CItemSlotUI* grabSlotUI = mItemSlotUIs[mGrabSlotIndex];
	CItemSlotUI* enterSlotUI = mItemSlotUIs[mEnterSlotIndex];

	// 掴んでいるアイテムのスロットの情報を保持しておく
	const ItemData* tmpData = grabData.data;
	int tmpCount = grabData.count;

	// 掴んでいたアイテムスロットに、
	// カーソルが重なっていたアイテムスロットのデータを入れる
	grabData.data = enterData.data;
	grabData.count = enterData.count;
	grabSlotUI->SetItemSloto(grabData.data, grabData.count);

	// カーソルが重なっていたアイテムスロットには、
	// 掴んでいたアイテムスロットのデータを入れる
	enterData.data = tmpData;
	enterData.count = tmpCount;
	enterSlotUI->SetItemSloto(enterData.data, enterData.count);

	//// プレイヤーがアイテムを装備しているか
	//CAdventurer* adventurer = CAdventurer::Instance();
	//int equipSlotIndex = adventurer->GetEquipItemSlotIndex();
	//if (equipSlotIndex >= 0)
	//{
	//	// 掴んで移動したアイテムが装備アイテムであれば、
	//	// 移動後のスロットのアイテムを装備アイテムとして設定
	//	if (equipSlotIndex == mGrabSlotIndex) adventurer->EquipItem(mEnterSlotIndex);
	//	// 離したスロットのアイテムが装備アイテムであれば、
	//	// 掴んだスロットと入れ替えたアイテムを装備アイテムとして設定
	//	else if (equipSlotIndex == mEnterSlotIndex) adventurer->EquipItem(mGrabSlotIndex);
	//}

	// 掴んでいた状態を解除
	mGrabSlotIndex = -1;
}

// 更新
void CInventory::Update()
{
	// アイテムスロットのイメージの更新
	for (CItemSlotUI* slotUI : mItemSlotUIs)
	{
		slotUI->Update();
	}

	if (mEnterSlotIndex != -1)
	{
		//SlotData& itemData = CGameData::ItemSlots[mEnterSlotIndex];
		CItemSlotUI* slotUI = mItemSlotUIs[mEnterSlotIndex];

		//// アイテムアイコンの上で右クリックしたら
		//if (CInput::Key(VK_RBUTTON) && !mpItemMenu->IsShow())
		//{
		//	// アイテムアイコンがある場合
		//	if (itemData.data != nullptr)
		//	{
		//		mpItemMenu->SetPos(itemData.slotUI->GetPos());
		//		// アイテムメニューを表示する
		//		mpItemMenu->Open();
		//		mpItemMenu->Update();
		//	}
		//}
		//else
		//{
		mpSlotHighlight->SetPos(slotUI->GetPos().X(), slotUI->GetPos().Y());
		mpSlotHighlight->SetEnable(true);
		mpSlotHighlight->Update();
		//}
	}
	else if (mEnterSlotIndex == -1)
	{
		mpSlotHighlight->SetEnable(false);
	}

}

// 描画
void CInventory::Render()
{
	mpMenuBg->Render();
	mpInventoryFrame->Render();

	if (mpSlotHighlight->IsEnable())
	{
		mpSlotHighlight->Render();
	}

	// アイテムスロットのアイテムの描画
	int count = CGameData::ItemSlots.size();
	for (int i = 0; i < count; i++)
	{
		SlotData& slot = CGameData::ItemSlots[i];
		// 空のスロットは描画しない
		if (slot.data == nullptr) continue;
		// 現在つまんでいるアイテムスロットはこのタイミングでは描画しない
		if (i == mGrabSlotIndex) continue;

		mItemSlotUIs[i]->Render();
	}

	// アイテムスロットを掴んでいる場合
	if (mGrabSlotIndex >= 0)
	{
		// 空スロットでなければ、このタイミングで描画
		SlotData& slot = CGameData::ItemSlots[mGrabSlotIndex];
		if (slot.data != nullptr)
		{
			mItemSlotUIs[mGrabSlotIndex]->Render();
		}
	}
}

