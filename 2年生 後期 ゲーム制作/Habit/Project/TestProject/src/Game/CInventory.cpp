#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値

#define INVENTORY_WIDTH 918.5f	// インベントリのX座標
#define INVENTORY_HEIGHT 360.0f	// インベントリのY座標
#define INVENTORY_SIDE 6		// インベントリの横の格納数
#define INVENTORY_FRAME 5		// インベントリの枠の幅

#define ITEM_WIDTH 750.0f		// アイテムを格納するX座標
#define ITEM_HEIGHT 290.0f		// アイテムを格納するY座標
#define ITEM_INTERVAL 60		// アイテムの間隔

// コンストラクタ
CInventory::CInventory()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
{
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

	// チョコ
	int ItemCount = 8;
	for (int i = 0; i < ItemCount; i++)
	{
		CImage* choco = new CImage
		(
			"Item\\choco\\choco.png",
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);

		choco->SetCenter(choco->GetSize() * 0.5f);

		int w = i % INVENTORY_SIDE;
		int h = i / INVENTORY_SIDE;
		float x = ITEM_WIDTH + INVENTORY_FRAME * (w + 1) + ITEM_INTERVAL * w;
		float y = ITEM_HEIGHT + INVENTORY_FRAME * (h + 1) + ITEM_INTERVAL * h;

		choco->SetPos(x, y);
		mItemList.push_back(choco);

	}
	

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CInventory::~CInventory()
{
	SAFE_DELETE(mpBackground);
	SAFE_DELETE(mpInventoryFrame);
	SAFE_DELETE(mpBackMenu);
	SAFE_DELETE(mpSelectFrame);

	int size = mItemList.size();
	for (int i = 0; i < size; i++)
	{
		CImage* item = mItemList[i];
		mItemList[i] = nullptr;
		SAFE_DELETE(item);
	}
	mItemList.clear();
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

void CInventory::AddItem()
{
}

// 更新
void CInventory::Update()
{
	mpBackground->Update();
	mpInventoryFrame->Update();
	mpBackMenu->Update();
	mpSelectFrame->Update();

	for (CImage* item : mItemList)
	{
		item->Update();
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

	for (int i = 0; i < mItemList.size(); i++)
	{
		CImage* item = mItemList[i];
		item->Render();
	}
}
