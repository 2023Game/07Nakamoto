#pragma once

// アイテムの種類
enum class ItemType
{
	None = -1,

	Choco,		// チョコレート
	Cookie,		// クッキー

	Stone,		// 小石

	Num,
};

// アイテムの効果の種類
enum class ItemEffectType
{
	None = -1,		

	RecoveryHP,		// HP回復

	Weapon,			// 武器

	Throw,			// 投擲アイテム

	Num,
};

struct ItemData
{
	std::string name;		// アイテム名
	std::string desc;		// アイテムの説明文

	std::string iconPath;	// アイテム画像のパス
	std::string modelPath;	// モデルデータのパス

	ItemEffectType effectType;	// アイテムの効果の種類
	int recovery;		// 回復量
	int attack;			// 攻撃力
};

namespace Item
{
	// アイテムの種類を指定して、アイテムのデータを取得
	bool GetItemData(ItemType type, const ItemData** data);

	// 全てのアイテムのリソースを読み込む
	void LoadItemResources();
	// 全てのアイテムのリソースを破棄
	void DeleteItemResources();
}