#pragma once
#include "ItemData.h"

// アイテムのマネージャークラス
class CItemManager
{
public:
	// インスタンス
	static CItemManager* Instance();
	// インスタンスの削除
	static void ClearInstance();

	// フィールドのアイテムの生成
	void SpawnItems();

	// 妖力の源の生成
	void SpawnDemonPowers();
	// 呪符を生成
	void SpawnCharm(ItemType type, int num);

private:
	// コンストラクタ
	CItemManager();
	// デストラクタ
	~CItemManager();

	static CItemManager* mpInstance;
	std::vector<CVector> mCharmPositions;	// アイテムの配置のリスト
	std::vector<CVector> mDemonPowerPositions;	// 妖力の源の配列のリスト
};