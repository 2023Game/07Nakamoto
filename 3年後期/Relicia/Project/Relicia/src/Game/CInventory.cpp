#include "CInventory.h"

const std::vector<std::vector<int>>& CInventory::GetGrid() const
{
	return mGrid;
}

bool CInventory::IsInside(int x, int y) const
{
	return false;
}
