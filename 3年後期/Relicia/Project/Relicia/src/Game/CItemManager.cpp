#include "CItemManager.h"
#include "CField.h"
#include "CItemObj.h"

CItemManager* CItemManager::mpInstance = nullptr;

// インスタンスの生成
CItemManager* CItemManager::Instance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CItemManager();
	}

	return mpInstance;
}

// コンストラクタ
CItemManager::CItemManager()
{
	mpInstance = this;
}

// デストラクタ
CItemManager::~CItemManager()
{
	if (mpInstance == this)
	{
		mpInstance = nullptr;
	}
}

// インスタンスの削除
void CItemManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}


// 生成しているアイテムリストを取得
std::vector<CItemObj*>& CItemManager::GetItemObjs()
{
	return mpItemObjs;
}

// フィールドのアイテムの生成
void CItemManager::SpawnItems()
{
	// 初期化
	for (CItemObj* item : mpItemObjs) item->Kill();
	mpItemObjs.clear();

	// とりあえず4回生成
	for (int i = 0; i < 4; ++i)
	{
		mpItemObjs.push_back(new CItemObj(ItemId::Key, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
		mpItemObjs.push_back(new CItemObj(ItemId::HealingPotion, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
	}
}

// アイテム管理リストに追加
void CItemManager::AddItem(CItemObj* item)
{
	mpItemObjs.push_back(item);
}

// アイテム管理リストから削除
void CItemManager::RemoveItem(CItemObj* item)
{
	auto find = std::find(mpItemObjs.begin(), mpItemObjs.end(), item);
	if (find == mpItemObjs.end()) return;
	mpItemObjs.erase(find);
}

// 全てのアイテムを削除する
void CItemManager::AllRemoveItems()
{
	for (CItemObj* item : mpItemObjs)
	{
		if (item == nullptr) continue;

		item->Kill();
	}
	mpItemObjs.clear();
}

