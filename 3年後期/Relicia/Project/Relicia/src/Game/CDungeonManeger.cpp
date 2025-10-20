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

    //ConnectRooms();
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

//// 部屋と部屋の出入口を繋げる
//void CDungeonManeger::ConnectRooms()
//{
//    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
//    {
//        for (int x = 0; x < DUNGEON_SECTION_X; x++)
//        {
//            CDungeonMap* current = mpSections[y][x];
//
//            // 東側との接続
//            if (x < DUNGEON_SECTION_X - 1)
//            {
//                CDungeonMap* east = mpSections[y][x + 1];
//                ConnectBetweenRooms(current, east, CDungeonMap::Direction::eEast);
//            }
//            // 南側との接続
//            if (y < DUNGEON_SECTION_Y - 1)
//            {
//                CDungeonMap* south = mpSections[y + 1][x];
//                ConnectBetweenRooms(current, south, CDungeonMap::Direction::eSouth);
//            }
//        }
//    }
//}
//
//// 通路をを繋げる
//void CDungeonManeger::ConnectBetweenRooms(CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir)
//{
//    auto entrancesA = entranceA->GetEntrances();
//    auto entrancesB = entranceB->GetEntrances();
//
//    if (entrancesA.empty() || entrancesB.empty()) return;
//
//    // 一番近いペアを探す
//    CDungeonMap::Point bestA, bestB;
//    float bestDist = FLT_MAX;
//
//    for (auto& ea : entrancesA)
//    {
//        for (auto& eb : entrancesB)
//        {
//            float dx = (float)(ea.x - eb.x);
//            float dy = (float)(ea.y - eb.y);
//            float dist = dx * dx + dy * dy;
//            if (dist < bestDist)
//            {
//                bestDist = dist;
//                bestA = ea;
//                bestB = eb;
//            }
//        }
//    }
//    // 通路を作る
//    CreateCorridorBetween(bestA, bestB, entranceA, entranceB, dir);
//}
//
//// 部屋同士の間の通路を作成
//void CDungeonManeger::CreateCorridorBetween(const CDungeonMap::Point& aPos, const CDungeonMap::Point& bPos, 
//    CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir)
//{
//    if (dir == CDungeonMap::Direction::eEast)
//    {
//        // entranceAの右端の出入口 → entranceBの左端の出入口をつなぐ
//        for (int x = aPos.x + 1; x < ROOM_WIDTH; ++x)
//        {
//            entranceA->SetTileType(aPos.y, x, CDungeonMap::TileType::eFloor);
//        }
//
//        for (int x = 0; x <= bPos.x; ++x)
//        {
//            entranceB->SetTileType(bPos.y, x, CDungeonMap::TileType::eFloor);
//        }
//    }
//    else if (dir == CDungeonMap::Direction::eSouth)
//    {
//        // entranceAの下端の出入口 → entranceBの上端の出入口をつなぐ
//        for (int y = aPos.y + 1; y < ROOM_HEIGHT; ++y)
//        {
//            entranceA->SetTileType(y, aPos.x, CDungeonMap::TileType::eFloor);
//        }
//
//        for (int y = 0; y <= bPos.y; ++y)
//        {
//            entranceB->SetTileType(y, bPos.x, CDungeonMap::TileType::eFloor);
//        }
//    }
//}
