#pragma once

class CImage;

// アイテムの種類
enum class ItemType
{
	None = -1,

	HealingPotion,		// 回復薬
	Bread,				// パン
	Key,				// 鍵

	Num,
};

// アイテムの効果の種類
enum class ItemEffectType
{
	None = -1,

	RecoveryHP,		// HP回復
	Key,			// 鍵

	Num,
};

struct Rect
{
	int x;
	int y;
	int w;
	int h;
};

struct SpriteIndex
{
	int col;	// 横何番目
	int row;	// 縦何番目
};

// アイテムのデータ
struct ItemData
{
	ItemType type;			// アイテムの種類
	int slotLimit;			// アイテムスロットに入る上限

	std::string name;		// アイテム名
	std::string desc;		// アイテムの説明文

	std::string iconSheetPath; // 共通のスプライトシート
	SpriteIndex iconIndex;     // 切り出し番号

	//std::string iconPath;	// アイテム画像のパス
	std::string modelPath;	// モデルデータのパス

	ItemEffectType effectType;	// アイテムの効果の種類
	int recovery;		// 回復量
	int attack;			// 攻撃力

	//int width;	// 占有範囲の横幅
	//int height;	// 占有範囲の縦幅
	//bool shape[MAX_SHAPE][MAX_SHAPE];	// 占有マスク
};

namespace Item
{
	// アイテムの種類を指定して、アイテムのデータを取得
	bool GetItemData(ItemType type, const ItemData** data);

	// 全てのアイテムのリソースを読み込む
	void LoadItemResources();
	// 全てのアイテムのリソースを破棄
	void DeleteItemResources();
	// 切り取るアイコンの場所を計算
	Rect CalcIconRect(const SpriteIndex& index);
	// 切り取るアイコンの座標をUVに変換
	CVector4 RectToUV(int x, int y, int w, int h, int texW, int texH);
}