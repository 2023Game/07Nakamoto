#include "CInventory2.h"


// コンストラクタ
CInventory2::CInventory2(int width, int height)
{
}

// デストラクタ
CInventory2::~CInventory2()
{
}

// アイテムがその位置に置けるか判定
bool CInventory2::CanPlace(const ItemInstance& item, int x, int y) const
{
	return false;
}

// アイテムの追加
bool CInventory2::AddItem(const int ItemInstance, int startX, int startY)
{
	return false;
}

// 配置されているアイテムを削除
void CInventory2::RemoveItem(int itemId)
{
}

// 配置アイテムを回転(90度)
void CInventory2::RotateItem(int itemId)
{
}

// 配置アイテムを左右反転
void CInventory2::FlipItem(int itemId)
{
}

// インベントリを拡張(装備などで増加)
void CInventory2::Expand(int addWidth, int addHeight)
{
}

// インベントリのグリッドを取得（UI用）
const std::vector<std::vector<int>>& CInventory2::GetGrid() const
{
	return mGrid;
}

// グリッド内判定
bool CInventory2::IsInside(int x, int y) const
{
	return false;
}

