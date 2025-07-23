#pragma once

// アイテムの種類
enum class ItemType
{
	None = -1,

	FlameCharm,		// 火炎の呪符
	BindingCharm,	// 束縛の呪符
	BarrierCharm,	// 結界の呪符
	HealingCharm,	// 治癒の呪符
	SubstituteCharm,// 身代わりの呪符

	Key,			// 鍵

	Num,
};

// アイテムの効果の種類
enum class ItemEffectType
{
	None = -1,

	RecoveryHP,		// HP回復
	Weapon,			// 武器
	Throw,			// 投擲アイテム
	Installation,	// 設置アイテム
	Substitute,		// 身代わりアイテム

	Num,
};

// アイテムのデータ
struct ItemData
{
	ItemType type;			// アイテムの種類
	int slotLimit;			// アイテムスロットに入る上限

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