#include "CDungeonManeger.h"
#include "CDungeonMap.h"

// コンストラクタ
CDungeonManeger::CDungeonManeger()
{
    // 全体の区画の初期化
    Initialize();

    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        for (int x = 0; x < DUNGEON_SECTION_X; x++)
        {
            mpSections[y][x] = new CDungeonMap(); // 各区画の部屋生成

            // 縦方向に次の部屋が存在する場合は、
            // 次の部屋の方向（南側）の壁の中から、ランダムで出入り口を作る
            if (y < DUNGEON_SECTION_Y - 1)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eNorth);
            }
            // 縦方向に前の部屋が存在する場合は、
            // 前の部屋の方向（北側）の壁の中から、ランダムで出入り口を作る
            if (y > 0)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eSouth);
            }

            // 横方向に次の部屋が存在する場合は、
            // 次の部屋の方向（東側）の壁の中から、ランダムで出入り口を作る
            if (x < DUNGEON_SECTION_X - 1)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eWest);
            }
            // 横方向に前の部屋が存在する場合は、
            // 前の部屋の方向（西側）の壁の中から、ランダムで出入り口を作る
            if (x > 0)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eEast);
            }
        }
    }
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

// 全体区画の初期化
void CDungeonManeger::Initialize()
{
    mpSections.resize(DUNGEON_SECTION_Y);

    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        mpSections[y].resize(DUNGEON_SECTION_X);
    }
}

// 区画の取得
const CDungeonMap* CDungeonManeger::GetSection(int x, int y) const
{
    return mpSections[y][x];
}