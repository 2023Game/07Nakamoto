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
	// 部屋と部屋の出入口を繋げる
	//void ConnectRooms();

	/// <summary>
	/// 通路をを繋げる
	/// </summary>
	/// <param name="aEntrance">繋げる出入口A</param>
	/// <param name="bEntrance">繋げる出入口B</param>
	/// <param name="dir">方角</param>
	//void ConnectBetweenRooms(CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir);

	/// <summary>
	/// 部屋同士の間の通路を作成
	/// </summary>
	/// <param name="aPos"></param>
	/// <param name="bPos"></param>
	/// <param name="entranceA">繋げる出入口A</param>
	/// <param name="entranceB">繋げる出入口B</param>
	/// <param name="dir">方角</param>
	//void CreateCorridorBetween(
	//	const CDungeonMap::Point& aPos, const CDungeonMap::Point& bPos,
	//	CDungeonMap* entranceA, CDungeonMap* entranceB,
	//	CDungeonMap::Direction dir);

	// 全体の区画の配列
	CDungeonMap* mpSections[DUNGEON_SECTION_Y][DUNGEON_SECTION_X];
};