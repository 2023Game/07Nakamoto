#pragma once
#include "CDungeonMap.h"

#define DUNGEON_SECTION_X 4	// 横の区画数
#define DUNGEON_SECTION_Y 4	// 縦の区画数

class CDungeonManeger
{
public:
	// コンストラクタ
	CDungeonManeger();
	// デストラクタ
	~CDungeonManeger();

	// ダンジョン生成
	void GenerateDungeon();
	// 区画の取得
	const CDungeonMap* GetSection(int x, int y) const;

private:
	// 出入口同士を繋げる通路を生成
	void ConnectRooms();

	CDungeonMap* mpSections[DUNGEON_SECTION_Y][DUNGEON_SECTION_X];
};