#include "CDungeonManeger.h"
#include "CDungeonMap.h"

// �R���X�g���N�^
CDungeonManeger::CDungeonManeger()
{
    // �S�̂̋��̏�����
    Initialize();

    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        for (int x = 0; x < DUNGEON_SECTION_X; x++)
        {
            mpSections[y][x] = new CDungeonMap(); // �e���̕�������
        }
    }
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

// �S�̋��̏�����
void CDungeonManeger::Initialize()
{
    mpSections.resize(DUNGEON_SECTION_Y);

    for (int y = 0; y < DUNGEON_SECTION_Y; y++)
    {
        mpSections[y].resize(DUNGEON_SECTION_X);
    }
}

// ���̎擾
const CDungeonMap* CDungeonManeger::GetSection(int x, int y) const
{
    return mpSections[y][x];
}