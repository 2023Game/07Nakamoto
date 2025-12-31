#include "CInteractObjectManager.h"
#include "CInteractObject.h"
#include <assert.h>

#define RESERVED_CAPACITYE 10	// リストの初期容量

// インスタンス
CInteractObjectManager* CInteractObjectManager::spInstance = nullptr;

// インスタンスを取得
CInteractObjectManager* CInteractObjectManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CInteractObjectManager::CInteractObjectManager()
{
	assert(spInstance == nullptr);
	spInstance = this;

	// リストの初期容量の設定
	mInteractObjects.reserve(RESERVED_CAPACITYE);
}

// デストラクタ
CInteractObjectManager::~CInteractObjectManager()
{
	spInstance = nullptr;
}

// インスタンスを破棄
void CInteractObjectManager::CIearInstance()
{
	SAFE_DELETE(spInstance);
}

// 調べるオブジェクトを追加
void CInteractObjectManager::Add(CInteractObject* obj)
{
	mInteractObjects.push_back(obj);
}

// 調べるオブジェクトを取り除く
void CInteractObjectManager::Remove(CInteractObject* obj)
{
	auto result = std::remove(mInteractObjects.begin(), mInteractObjects.end(), obj);
	mInteractObjects.erase(result, mInteractObjects.end());
}

// 指定した座標に近い調べるオブジェクトを返す
bool CInteractObjectManager::GetNearInteractObjects(const CVector& pos, float distance,
	std::vector<CInteractObject*>& list)
{
	list.clear();
	for (CInteractObject* obj : mInteractObjects)
	{
		// 調べるオブジェクトが有効でない場合は、スルー
		if (!obj->IsEnable()) continue;
		// 調べるオブジェクトの衝突範囲がオフの場合は、スルー
		if (!obj->IsEnableCol()) continue;
		// 調べるオブジェクトの削除フラグが立っている場合は、スルー
		if (obj->IsKill()) continue;

		// 調べるオブジェクトと指定された座標の距離を取得
		float dist = (obj->GetInteractPos() - pos).LengthSqr();
		// 求めた距離が指定された距離より遠ければ、次の調べるオブジェクトへ進む
		if (dist > distance * distance) continue;

		// リストに調べるオブジェクトを追加
		list.push_back(obj);
	}
	// リストの中身が1個以上あれば、近くのオブジェクトが見つかった
	return list.size() > 0;
}
