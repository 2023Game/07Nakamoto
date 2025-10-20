#include "CDungeonManeger.h"

// �R���X�g���N�^
CDungeonManeger::CDungeonManeger()
{
    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        for (int x = 0; x < DUNGEON_SECTION_X; x++)
        {
            mpSections[y][x] = new CDungeonMap(); // �e���̕�������
        }
    }

    //ConnectRooms();
}

// �f�X�g���N�^
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

// �_���W��������
void CDungeonManeger::GenerateDungeon()
{
}

// ���̎擾
const CDungeonMap* CDungeonManeger::GetSection(int x, int y) const
{
    return mpSections[y][x];
}

//// �����ƕ����̏o�������q����
//void CDungeonManeger::ConnectRooms()
//{
//    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
//    {
//        for (int x = 0; x < DUNGEON_SECTION_X; x++)
//        {
//            CDungeonMap* current = mpSections[y][x];
//
//            // �����Ƃ̐ڑ�
//            if (x < DUNGEON_SECTION_X - 1)
//            {
//                CDungeonMap* east = mpSections[y][x + 1];
//                ConnectBetweenRooms(current, east, CDungeonMap::Direction::eEast);
//            }
//            // �쑤�Ƃ̐ڑ�
//            if (y < DUNGEON_SECTION_Y - 1)
//            {
//                CDungeonMap* south = mpSections[y + 1][x];
//                ConnectBetweenRooms(current, south, CDungeonMap::Direction::eSouth);
//            }
//        }
//    }
//}
//
//// �ʘH�����q����
//void CDungeonManeger::ConnectBetweenRooms(CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir)
//{
//    auto entrancesA = entranceA->GetEntrances();
//    auto entrancesB = entranceB->GetEntrances();
//
//    if (entrancesA.empty() || entrancesB.empty()) return;
//
//    // ��ԋ߂��y�A��T��
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
//    // �ʘH�����
//    CreateCorridorBetween(bestA, bestB, entranceA, entranceB, dir);
//}
//
//// �������m�̊Ԃ̒ʘH���쐬
//void CDungeonManeger::CreateCorridorBetween(const CDungeonMap::Point& aPos, const CDungeonMap::Point& bPos, 
//    CDungeonMap* entranceA, CDungeonMap* entranceB, CDungeonMap::Direction dir)
//{
//    if (dir == CDungeonMap::Direction::eEast)
//    {
//        // entranceA�̉E�[�̏o���� �� entranceB�̍��[�̏o�������Ȃ�
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
//        // entranceA�̉��[�̏o���� �� entranceB�̏�[�̏o�������Ȃ�
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
