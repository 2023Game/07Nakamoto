#include "CDungeonManeger.h"

// コンストラクタ
CDungeonManeger::CDungeonManeger()
{
    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        for (int x = 0; x < DUNGEON_SECTION_X; x++)
        {
            mpSections[y][x] = new CDungeonMap(); // 各区画の部屋生成
        }
    }

    ConnectRooms();
}

// デストラクタ
CDungeonManeger::~CDungeonManeger()
{
    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        for (int x = 0; x < DUNGEON_SECTION_X; x++)
        {
            delete mpSections[y][x];
            mpSections[y][x] = nullptr;
        }
    }
}

// ダンジョン生成
void CDungeonManeger::GenerateDungeon()
{
}

// 区画の取得
const CDungeonMap* CDungeonManeger::GetSection(int x, int y) const
{
    return mpSections[y][x];
}

// 出入口同士を繋げる通路を生成
void CDungeonManeger::ConnectRooms()
{
}
