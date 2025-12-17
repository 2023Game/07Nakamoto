#pragma once
#include "CRect.h"

class CImage;

// アイテムのID
enum class ItemId
{
	None = -1,

	HealingPotion,		// 回復薬
	HealingPotion2,		// 回復薬
	HealingPotion3,		// 回復薬
	Bomb,				// 爆弾
	Bread,				// パン
	Chicken,			// チキン
	Key,				// 鍵
	Key2,				// 鍵
	Key3,				// 鍵

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
	ItemId id;			// アイテムの種類
	int slotLimit;			// アイテムスロットに入る上限

	std::string name;		// アイテム名
	std::string desc;		// アイテムの説明文

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
	bool GetItemData(ItemId type, const ItemData** data);

	// 全てのアイテムのリソースを読み込む
	void LoadItemResources();
	// 全てのアイテムのリソースを破棄
	void DeleteItemResources();
	// 切り取るアイコンの場所を計算
	CRect CalcIconUV(ItemId id, const CVector2& texSize);
	// 切り取るアイコンの座標をUVに変換
	CVector4 RectToUV(int x, int y, int w, int h, int texW, int texH);
}