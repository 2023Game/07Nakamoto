#pragma once

class CItemPositions
{
public:
	// コンストラクタ
	CItemPositions(std::vector<CVector> positions);
	// デストラクタ
	~CItemPositions();

private:
	std::vector<CVector> mPositions;	// アイテムの配置のリスト
};