#include "CInventory.h"
#include "CInput.h"
#include "CTaskManager.h"
#include "CBGMManager.h"

#define INVENTORY_ALPHA 0.75f	// アルファ値

// コンストラクタ
CInventory::CInventory()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eMenu)
	, mSelectIndex(0)
	, mIsOpened(false)
{
	mpBackground = new CImage
	(
		"UI\\menu_back.ping",
		ETaskPriority::eUI, 0, ETaskPauseType::eMenu,
		false, false
	);
	mpBackground->SetCenter(mpBackground->GetSize() * 0.5f);
	mpBackground->SetPos(CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f);
	mpBackground->SetColor(1.0f, 1.0f, 1.0f, INVENTORY_ALPHA);


}

// デストラクタ
CInventory::~CInventory()
{
}

// 開く
void CInventory::Open()
{
}

// 閉じる
void CInventory::Close()
{
}

// インベントリが開いているかどうか
bool CInventory::IsOpend() const
{
	return false;
}

// 決める
void CInventory::Decide(int select)
{
}

// 更新
void CInventory::Update()
{
}

// 描画
void CInventory::Render()
{
}
