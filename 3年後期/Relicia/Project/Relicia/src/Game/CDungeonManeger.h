#pragma once

#define DUNGEON_SECTION_X 4	// 横の区画数
#define DUNGEON_SECTION_Y 4	// 縦の区画数

class CDungeonMap;

class CDungeonManeger
{
public:
	// コンストラクタ
	CDungeonManeger();
	// デストラクタ
	~CDungeonManeger();

	// 全体区画の初期化
	void Initialize();

	// 区画の取得
	const CDungeonMap* GetSection(int x, int y) const;

private:
	// 全体の区画をリストに格納
	std::vector<std::vector<CDungeonMap*>>mpSections;
};