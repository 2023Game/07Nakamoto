#include "CCrystalManager.h"
#include "CField.h"
#include "CCrystalObj.h"

CCrystalManager* CCrystalManager::spInstance = nullptr;

// インスタンス
CCrystalManager* CCrystalManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CCrystalManager();
	}

	return spInstance;
}

// インスタンス
CCrystalManager::CCrystalManager()
{
	spInstance = this;
}

// デストラクタ
CCrystalManager::~CCrystalManager()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// インスタンスの削除
void CCrystalManager::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// 生成しているクリスタルリストを取得
std::vector<CCrystalObj*>& CCrystalManager::GetItemObjs()
{
	return mpCrystalObjs;
}

// フィールドのクリスタルの生成
void CCrystalManager::SpawnCrystals()
{
	// 初期化
	AllRemoveCrystals();

	// とりあえず4回生成
	for (int i = 0; i < 4; ++i)
	{
		AddCrystal(new CCrystalObj(ElementType::Fire, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
		AddCrystal(new CCrystalObj(ElementType::Water, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
		AddCrystal(new CCrystalObj(ElementType::Thunder, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
		AddCrystal(new CCrystalObj(ElementType::Wind, CField::Instance()->GetMapData()->GetRoomRandomFloorPos()));
	}
}

// クリスタル管理リストに追加
void CCrystalManager::AddCrystal(CCrystalObj* crystal)
{
	mpCrystalObjs.push_back(crystal);
}

// クリスタル管理リストから削除
void CCrystalManager::RemoveCrystal(CCrystalObj* crystal)
{
	auto find = std::find(mpCrystalObjs.begin(), mpCrystalObjs.end(), crystal);
	if (find == mpCrystalObjs.end()) return;
	mpCrystalObjs.erase(find);
}

// 全てのクリスタルを削除する
void CCrystalManager::AllRemoveCrystals()
{
	for (CCrystalObj* crystal : mpCrystalObjs)
	{
		if (crystal && !crystal->IsKill())
		{
			crystal->Kill();
		}
	}
	mpCrystalObjs.clear();
}

// インスタンスを破棄する
void CCrystalManager::Destroy()
{
	delete spInstance;
	spInstance = nullptr;
}

