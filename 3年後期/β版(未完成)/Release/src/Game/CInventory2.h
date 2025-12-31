#pragma once
#include "ItemData.h"

#define MAX_SHAPE_SIZE 5	// 最大の形

class CInventory2
{
public:
	// コンストラクタ
	CInventory2(int width = 5, int height = 6);
	// デストラクタ
	~CInventory2();

	// 回転・反転後の形状を含む個別のアイテムデータ
	struct ItemInstance
	{
		int id;             // 個体識別用（ユニークID）
		const ItemData* data;	// 静的データへの参照
		bool rotatedShape[MAX_SHAPE_SIZE][MAX_SHAPE_SIZE];	// 現在の形状
		int width;
		int height;
	};

	// アイテムがその位置に置けるか判定
	bool CanPlace(const ItemInstance& item, int x, int y) const;
	// アイテムの追加
	bool AddItem(const int ItemInstance, int startX, int startY);
	// 配置されているアイテムを削除
	void RemoveItem(int itemId);
	// 配置アイテムを回転(90度)
	void RotateItem(int itemId);
	// 配置アイテムを左右反転
	void FlipItem(int itemId);
	// インベントリを拡張(装備などで増加)
	void Expand(int addWidth, int addHeight);
	// インベントリのグリッドを取得（UI用）
	const std::vector<std::vector<int>>& GetGrid() const;

private:
	// 配置済みアイテム情報（再描画＆取り外しに使用）
	struct PlacedInfo
	{
		int x, y; // 左上の位置
		int width, height;
		bool shape[MAX_SHAPE_SIZE][MAX_SHAPE_SIZE];
	};

	int mWidth;
	int mHeight;

	// グリッド：アイテムIDを格納（0は空）
	std::vector<std::vector<int>> mGrid;
	// 置いてあるアイテムの情報
	std::map<int, PlacedInfo> mPlacedItems;

	// グリッド内判定
	bool IsInside(int x, int y) const;
};