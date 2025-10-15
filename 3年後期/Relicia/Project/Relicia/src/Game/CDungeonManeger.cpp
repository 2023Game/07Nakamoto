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

    ConnectRooms();
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

// �o�������m���q����ʘH�𐶐�
void CDungeonManeger::ConnectRooms()
{
}
