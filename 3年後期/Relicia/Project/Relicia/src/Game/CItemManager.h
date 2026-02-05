#pragma once
#include "ItemData.h"

class CItemObj;

// アイテムのマネージャークラス
class CItemManager
{
public:
	// インスタンス
	static CItemManager* Instance();
	// インスタンスの削除
	static void ClearInstance();

	// 生成しているアイテムリストを取得
	std::vector<CItemObj*>& GetItemObjs();
	// フィールドのアイテムの生成
	void SpawnItems();
	// アイテム管理リストに追加
	void AddItem(CItemObj* item);
	// アイテム管理リストから削除
	void RemoveItem(CItemObj* item);
	// 全てのアイテムを削除する
	void AllRemoveItems();
	// インスタンスを破棄する
	void Destroy();

private:
	// コンストラクタ
	CItemManager();
	// デストラクタ
	~CItemManager();

	static CItemManager* spInstance;	// インスタンス
	std::vector<CItemObj*> mpCrystalObjs;	// アイテムの配置のリスト
};