#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値

#define INVENTORY_WIDTH 918.5f	// インベントリのX座標
#define INVENTORY_HEIGHT 360.0f	// インベントリのY座標

#define ITEM_WIDTH 750.0f		// アイテムを格納するX座標
#define ITEM_HEIGHT 290.0f		// アイテムを格納するY座標
#define ITEM_INTERVAL 60		// アイテムの間隔

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
	, mSelectIndex(0)
	, mIsOpened(false)
{
	spInstance = this;

	// 各アイテムスロットのアイテムアイコン表示用のイメージを作成
	int slotCount = SLOT_COUNT;
	for (int i = 0; i < slotCount; i++)
	{
		SlotData& slot = mItemSlots[i];
		slot.icon = new CImage
		(
			"", ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);
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

	// チョコ インベントリに入る処理
	//int ItemCount = 8;
	//for (int i = 0; i < ItemCount; i++)
	//{
	//	CImage* choco = new CImage
	//	(
	//		"Item\\choco\\choco.png",
	//		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
	//		false, false
	//	);

	//	choco->SetCenter(choco->GetSize() * 0.5f);

	//	int w = i % SLOT_COLUMN;
	//	int h = i / SLOT_COLUMN;
	//	float x = ITEM_WIDTH + SLOT_FRAME * (w + 1) + ITEM_INTERVAL * w;
	//	float y = ITEM_HEIGHT + SLOT_FRAME * (h + 1) + ITEM_INTERVAL * h;

	//	choco->SetPos(x, y);
	//	mItemSlots.push_back(choco);

	//}
	

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

	for (SlotData& slot : mItemSlots)
	{
		SAFE_DELETE(slot.icon);
	}

	//int size = mItemList.size();
	//for (int i = 0; i < size; i++)
	//{
	//	CImage* item = mItemList[i];
	//	mItemList[i] = nullptr;
	//	SAFE_DELETE(item);
	//}
	//mItemList.clear();
}

// 開く
void CInventory::Open()
{
	if (mIsOpened) return;

	// マウスカーソルを表示
	CInput::ShowCursor(true);

	SetEnable(true);
	SetShow(true);
	mSelectIndex = 0;
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
	switch (select)
	{
	case 0:
	case 1:
		break;
	case 2:
		Close();
		break;
	}
}

// アイテムを追加する
void CInventory::AddItem(ItemType type, int count)
{

}

// 更新
void CInventory::Update()
{
	mpBackground->Update();
	mpInventoryFrame->Update();
	mpBackMenu->Update();
	mpSelectFrame->Update();

	// アイテムスロットのイメージの更新
	for (SlotData& slot : mItemSlots)
	{
		slot.icon->Update();
	}


	//for (CImage* item : mItemList)
	//{
	//	item->Update();
	//}
}

// 描画
void CInventory::Render()
{
	mpBackground->Render();
	mpInventoryFrame->Render();
	mpBackMenu->Render();

	mpSelectFrame->SetPos(mpBackMenu->GetPos());
	mpSelectFrame->Render();

	// アイテムスロットを描画
	for (SlotData& slot : mItemSlots)
	{
		// 空のスロットは描画しない
		if (slot.data = nullptr) return;

		slot.icon->Render();
	}
	//for (int i = 0; i < mItemList.size(); i++)
	//{
	//	CImage* item = mItemList[i];
	//	item->Render();
	//}
}
