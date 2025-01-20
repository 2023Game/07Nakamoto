#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"
#include "CPlayer2.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値
#define MENU_ALPHA 0.75f

#define INVENTORY_WIDTH 918.5f	// インベントリのX座標
#define INVENTORY_HEIGHT 360.0f	// インベントリのY座標
#define ITEM_WIDTH 755.0f		// アイテムを格納するX座標
#define ITEM_HEIGHT 295.0f		// アイテムを格納するY座標

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
	mpBackMenu->SetColor(1.0f, 1.0f, 1.0f, MENU_ALPHA);

	// 選択されているボタンを強調する枠
	mpSelectFrame = new CImage
	(
		"UI/menu_item_select.png",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpSelectFrame->SetCenter(mpSelectFrame->GetSize() * 0.5f);
	mpSelectFrame->SetColor(1.0f, 0.5f, 0.0f, MENU_ALPHA);

	// チョコ
	int ItemCount = 3;
	for (int i = 0; i < ItemCount; i++)
	{
		CImage* choco = new CImage
		(
			"Item\\choco.png",
			ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
			false, false
		);

		choco->SetCenter(choco->GetSize() * 0.5f);

		int w = i % 6;
		int h = i / 6;
		float x = ITEM_WIDTH + 5 * (w + 1) + 30 * w;
		float y = ITEM_HEIGHT + 5 * (h + 1) + 30 * h;

		choco->SetPos(x, y * (i + 1));
		mItemList.push_back(choco);

		//mpChoco->SetPos(ITEM_WIDTH, ITEM_HEIGHT);
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


}

// 開く
void CInventory::Open()
{
	SetEnable(true);
	SetShow(true);
	mSelectIndex = 0;
	CBGMManager::Instance()->Play(EBGMType::eMenu, false);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// 閉じる
void CInventory::Close()
{
	SetEnable(false);
	SetShow(false);
	CBGMManager::Instance()->Play(EBGMType::eGame, false);
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
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
	mpChoco->Update();
}

// 描画
void CInventory::Render()
{
	mpBackground->Render();
	mpInventoryFrame->Render();
	mpBackMenu->Render();

	mpSelectFrame->SetPos(mpBackMenu->GetPos());
	mpSelectFrame->Render();
	mpChoco->Render();
}
