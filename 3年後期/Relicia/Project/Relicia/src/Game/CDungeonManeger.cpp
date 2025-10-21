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

            // �c�����Ɏ��̕��������݂���ꍇ�́A
            // ���̕����̕����i�쑤�j�̕ǂ̒�����A�����_���ŏo����������
            if (y < DUNGEON_SECTION_Y - 1)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eNorth);
            }
            // �c�����ɑO�̕��������݂���ꍇ�́A
            // �O�̕����̕����i�k���j�̕ǂ̒�����A�����_���ŏo����������
            if (y > 0)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eSouth);
            }

            // �������Ɏ��̕��������݂���ꍇ�́A
            // ���̕����̕����i�����j�̕ǂ̒�����A�����_���ŏo����������
            if (x < DUNGEON_SECTION_X - 1)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eWest);
            }
            // �������ɑO�̕��������݂���ꍇ�́A
            // �O�̕����̕����i�����j�̕ǂ̒�����A�����_���ŏo����������
            if (x > 0)
            {
                mpSections[y][x]->CreateRoomEntrance(CDungeonMap::Direction::eEast);
            }
        }
    }

#if _DEBUG
    PrintSection();
#endif
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

#if _DEBUG
// �S�̂̋��̃^�C���̃f�o�b�N�\��
void CDungeonManeger::PrintSection()
{
    // �S�̂̕��E���������߂�
    const int totalWidth = ROOM_WIDTH * DUNGEON_SECTION_X;
    const int totalHeight = ROOM_HEIGHT * DUNGEON_SECTION_Y;

    // �s���Ƃɏo��
    for (int globalY = 0; globalY < totalHeight; ++globalY)
    {
        for (int globalX = 0; globalX < totalWidth; ++globalX)
        {
            // �����W�����߂�
            int sectionX = globalX / ROOM_WIDTH;
            int sectionY = globalY / ROOM_HEIGHT;

            // �����̃��[�J�����W�����߂�
            int localX = globalX % ROOM_WIDTH;
            int localY = globalY % ROOM_HEIGHT;

            // �Ή���������擾
            CDungeonMap* map = mpSections[sectionY][sectionX];
            auto tile = map->GetTile(localX, localY);

            // ��ނ��Ƃɕ\��
            switch (tile.typeId)
            {
            case CDungeonMap::TileType::None:     printf("0"); break;
            case CDungeonMap::TileType::eFloor:   printf("1"); break;
            case CDungeonMap::TileType::eWall:    printf("2"); break;
            case CDungeonMap::TileType::ePillar:  printf("3"); break;
            case CDungeonMap::TileType::eEntrance:printf("4"); break;
            default:                              printf("?"); break;
            }
        }
        printf("\n");
    }
}
#endif

