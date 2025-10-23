#include "CBpsMap.h"
#include "Maths.h"

// ���̍ŏ��T�C�Y
#define MIN_SIZE 40
// �����̍ŏ��T�C�Y
#define MIN_ROOM 5

// ���̗]��(���̒[����1�}�X�󂯂�)
#define MARGIN 1

// �R���X�g���N�^
CBpsMap::CBpsMap(int x, int y)
{
    // ������
    Initialize(x, y);

    // �ċA�I�ɋ��𕪊�
    Split(mpRoot);

    // �e���ɕ�����z�u
    PlaceRoom(mpRoot, mMapData);

#if _DEBUG
    // �Q�����z��̃f�o�b�O�\��
    PrintSection();
#endif
}

// �f�X�g���N�^
CBpsMap::~CBpsMap()
{
    DeleteNode(mpRoot);
}

// �m�[�h�̍폜
void CBpsMap::DeleteNode(SectionNode* node)
{
    if (node == nullptr)
        return;
    DeleteNode(node->left);
    DeleteNode(node->right);
    delete node;
}

// ������
void CBpsMap::Initialize(int width, int height)
{
    mMapData.resize(height, std::vector<Room>(width, Room{ 0,0,0,0, CBpsMap::TileType::None }));

    // �ŏ��̑傫�ȋ��i�S�́j
    mpRoot = new SectionNode();
    mpRoot->x = 0;
    mpRoot->y = 0;
    mpRoot->width = width;
    mpRoot->height = height;
    mpRoot->left = nullptr;
    mpRoot->right = nullptr;
}

// ��敪��
void CBpsMap::Split(SectionNode* node)
{
    // �ݒ肵�Ă���T�C�Y�ȉ��Ȃ番�����Ȃ�
    if (node->width < MIN_SIZE && node->height < MIN_SIZE)
        return;

    bool splitVertical = (rand() % 2 == 0);

    if (splitVertical && node->width >= MIN_SIZE)
    {
        int splitX = Math::Rand(node->x + MIN_SIZE, node->x + node->width - MIN_SIZE);
        int left = splitX - node->x;
        int right = node->width - left;

        //if (left > right) return; // �����ł��Ȃ��̂ŏI��

        node->left = new SectionNode{ node->x, node->y, left, node->height, nullptr, nullptr };
        node->right = new SectionNode{ splitX, node->y, right, node->height, nullptr, nullptr };
    }
    else if (!splitVertical && node->height >= MIN_SIZE)
    {
        int splitY = Math::Rand(node->y + MIN_SIZE, node->y + node->height - MIN_SIZE);
        int top = splitY - node->y;
        int bottom = node->height - top;

        //if (top > bottom) return; // �����ł��Ȃ��̂ŏI��

        node->left = new SectionNode{ node->x, node->y, node->width, top, nullptr, nullptr };
        node->right = new SectionNode{ node->x, splitY, node->width, bottom, nullptr, nullptr };
    }
    else
    {
        return; // �ǂ���������ł��Ȃ�
    }

    // �ċA�I�ɕ���
    Split(node->left);
    Split(node->right);
}

// �����̐ݒ�
void CBpsMap::PlaceRoom(SectionNode* node, std::vector<std::vector<Room>>& map)
{
    if (node == nullptr)
        return;

    // �q�m�[�h��������΁i=�t�m�[�h�j���������
    if (node->left == nullptr && node->right == nullptr)
    {
        int maxRoomWidth = std::max(MIN_ROOM, node->width - MARGIN);
        int roomWidth = Math::Rand(MIN_ROOM, maxRoomWidth);
        int maxRoomHeight = std::max(MIN_ROOM, node->height - MARGIN);
        int roomHeight = Math::Rand(MIN_ROOM, maxRoomHeight);
        int roomX = Math::Rand(node->x + MARGIN, node->x + node->width - roomWidth - MARGIN);
        int roomY = Math::Rand(node->y + MARGIN, node->y + node->height - roomHeight - MARGIN);

        node->room = { roomX, roomY, roomWidth, roomHeight, TileType::eFloor };

        for (int y = roomY + 1; y < roomY + roomHeight - 1; y++)
            for (int x = roomX + 1; x < roomX + roomWidth - 1; x++)
                map[y][x].type = TileType::eFloor; // ���ɂ���
    }
    else
    {
        // �q�m�[�h�ɂ����������
        PlaceRoom(node->left, map);
        PlaceRoom(node->right, map);
    }
}

#if _DEBUG
// 2�����z��̃f�o�b�O�\��
void CBpsMap::PrintSection()
{
    for (int y = 0; y < mMapData.size(); y++)
    {
        for (int x = 0; x < mMapData[y].size(); x++)
        {
            switch (mMapData[y][x].type)
            {
            case TileType::None:        printf("0"); break;
            case TileType::eFloor:      printf("1"); break;
            case TileType::eWall:       printf("2"); break;
            case TileType::ePillar:     printf("3"); break;
            case TileType::eEntrance:   printf("4"); break;
            default:                    printf("?"); break;
            }
        }
        printf("\n");
    }
}
#endif
