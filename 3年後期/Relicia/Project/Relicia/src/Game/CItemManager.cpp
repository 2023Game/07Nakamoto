#include "CItemManager.h"
#include "CField.h"
#include "CItemObj.h"

CItemManager* CItemManager::spInstance = nullptr;

// インスタンスの生成
CItemManager* CItemManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CItemManager();
	}

	return spInstance;
}

// コンストラクタ
CItemManager::CItemManager()
{
	spInstance = this;
}

// デストラクタ
CItemManager::~CItemManager()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// インスタンスの削除
void CItemManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}


// 生成しているアイテムリストを取得
std::vector<CItemObj*>& CItemManager::GetItemObjs()
{
	return mpCrystalObjs;
}

// フィールドのアイテムの生成
void CItemManager::SpawnItems()
{
	// 初期化
	AllRemoveItems();

	// とりあえず4回生成
	for (int i = 0; i < 4; ++i)
	{
		mpCrystalObjs.push_back(new CItemObj(ItemId::Key, CField::Instance()->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Crystal)));
		mpCrystalObjs.push_back(new CItemObj(ItemId::HealingPotion, CField::Instance()->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Crystal)));
	}
}

// アイテム管理リストに追加
void CItemManager::AddItem(CItemObj* item)
{
	mpCrystalObjs.push_back(item);
}

// アイテム管理リストから削除
void CItemManager::RemoveItem(CItemObj* item)
{
	auto find = std::find(mpCrystalObjs.begin(), mpCrystalObjs.end(), item);
	if (find == mpCrystalObjs.end()) return;
	mpCrystalObjs.erase(find);
}

// 全てのアイテムを削除する
void CItemManager::AllRemoveItems()
{
	for (CItemObj* item : mpCrystalObjs)
	{
		if (item && !item->IsKill())
		{
			item->Kill();
		}
	}
	mpCrystalObjs.clear();
}

// インスタンスを破棄する
void CItemManager::Destroy()
{
	delete spInstance;
	spInstance = nullptr;
}

