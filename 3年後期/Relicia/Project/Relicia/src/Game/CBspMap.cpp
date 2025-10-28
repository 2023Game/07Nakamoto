#include "CBspMap.h"
#include "Maths.h"

// ���̍ŏ��T�C�Y
#define MIN_SIZE 10
// �����̍ŏ��T�C�Y
#define MIN_ROOM 5

// ���̗]��(���̒[����1�}�X�󂯂�)
#define MARGIN 1

// �R���X�g���N�^
CBspMap::CBspMap(int x, int y)
{
    // ������
    Initialize(x, y);

    // �ċA�I�ɋ��𕪊�
    Split(mpRoot);

#if _DEBUG
    // ���̋��E���ݒ�(���E�����݂����ꍇ�̂ݎg�p)
    DrawBoundary(mpRoot, mMapData);
#endif

    // �e���ɕ�����z�u
    PlaceRoom(mpRoot, mMapData);
    // �����K�w�̕������m��ʘH�Ōq����
    ConnectRooms(mpRoot, mMapData);

#if _DEBUG
    // �Q�����z��̃f�o�b�O�\��
    PrintSection();
#endif
}

// �f�X�g���N�^
CBspMap::~CBspMap()
{
    DeleteNode(mpRoot);
}


// BPS�}�b�v�f�[�^�̎擾
const std::vector<std::vector<CBspMap::Tile>>& CBspMap::GetTileData() const
{
    return mMapData;
}

// �m�[�h�̍폜
void CBspMap::DeleteNode(SectionNode* node)
{
    if (node == nullptr) return;

    DeleteNode(node->left);
    DeleteNode(node->right);
    delete node;
}

// ������
void CBspMap::Initialize(int width, int height)
{
    mMapData.resize(height, std::vector<Tile>(width));

    // �^�C�����̏�����
    for (auto& row : mMapData)
    {
        for (auto& tile : row)
        {
            tile = { TileType::None,Direction::eNorth };
        }
    }

    // �ŏ��̑傫�ȋ��̏������i�S�́j
    mpRoot = new SectionNode();
    mpRoot->x = 0;
    mpRoot->y = 0;
    mpRoot->width = width;
    mpRoot->height = height;
    mpRoot->left = nullptr;
    mpRoot->right = nullptr;
}

// ��敪��
void CBspMap::Split(SectionNode* node)
{
    // ���܂��͏c���ݒ肵�Ă���T�C�Y�ȉ��Ȃ番�����Ȃ�
    if (node->width < MIN_SIZE * 2 || node->height < MIN_SIZE * 2)
        return;

    // ���𕪂�����������߂�
    bool splitVertical;
    if (node->width > node->height)
    {
        splitVertical = true;
    }
    else if (node->height > node->width)
    {
        splitVertical = false;
    }
    else
    {
        splitVertical = Math::Rand(0, 1);
    }
    
    // true�Ȃ�c�ɕ���
    if (splitVertical && node->width >= MIN_SIZE * 2)
    {
        if (node->width < MIN_SIZE * 2 || node->height < MIN_SIZE * 2)
            return;

        int splitX = Math::Rand(node->x + MIN_SIZE, node->x + node->width - MIN_SIZE);
        int left = splitX - node->x;
        int right = node->width - left;

        node->left = new SectionNode{ node->x, node->y, left, node->height, nullptr, nullptr, {} };
        node->right = new SectionNode{ splitX, node->y, right, node->height, nullptr, nullptr, {} };

    }
    // false�Ȃ牡�ɕ���
    else if (!splitVertical && node->height >= MIN_SIZE * 2)
    {
        if (node->width < MIN_SIZE * 2 || node->height < MIN_SIZE * 2) 
            return;

        int splitY = Math::Rand(node->y + MIN_SIZE, node->y + node->height - MIN_SIZE);
        int top = splitY - node->y;
        int bottom = node->height - top;

        node->left = new SectionNode{ node->x, node->y, node->width, top, nullptr, nullptr, {} };
        node->right = new SectionNode{ node->x, splitY, node->width, bottom, nullptr, nullptr, {} };

    }

    // ����������Ă�����
    if (node->left != nullptr && node->right != nullptr)
    {
        // �ċA�I�ɕ���
        Split(node->left);
        Split(node->right);
    }
}

// �����̐ݒ�
void CBspMap::PlaceRoom(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    if (node == nullptr) return;

    // �q�m�[�h��������΁i=�t�m�[�h�j���������
    if (node->left == nullptr && node->right == nullptr)
    {
        // �����̍ŏ��T�C�Y + �����̃}�[�W�� < ���T�C�Y �̃`�F�b�N
        // ���̃`�F�b�N�ŁA�ȍ~�� Math::Rand �ň������t�]����\����啝�Ɍ��点��
        if (node->width < MIN_ROOM + MARGIN * 2 || node->height < MIN_ROOM + MARGIN * 2)
        {
            // ��悪���������邽�߁A��������炸�ɏI��
            return;
        }

        // �����̏��f�[�^�̐ݒ�
        CreateRoomFloor(node, map);
        // �����̕ǃf�[�^�̐ݒ�
        CreateRoomWall(node->room, map);
        // �����̎l���̒��f�[�^�̐ݒ�
        CreateRoomPillar(node->room, map);
    }
    else
    {
        // �q�m�[�h�ɂ����������
        PlaceRoom(node->left, map);
        PlaceRoom(node->right, map);
    }
}

// �����̏��f�[�^�̐ݒ�
void CBspMap::CreateRoomFloor(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    // 1. �����̃T�C�Y������ (�ő�l�� node->size - 2*MARGIN)
    int maxRoomWidth = node->width - (MARGIN * 2);
    int roomWidth = Math::Rand(MIN_ROOM, maxRoomWidth);

    int maxRoomHeight = node->height - (MARGIN * 2);
    int roomHeight = Math::Rand(MIN_ROOM, maxRoomHeight);

    if (maxRoomWidth < MIN_ROOM || maxRoomHeight < MIN_ROOM)
        return;

    // 2. �����̔z�u���W������ (Math::Rand�̈����̏�����ۏ�)

    // X���W
    int minRoomX = node->x + MARGIN;
    int maxRoomX = node->x + node->width - roomWidth - MARGIN;
    int roomX = Math::Rand(std::min(minRoomX, maxRoomX), std::max(minRoomX, maxRoomX));

    // Y���W
    int minRoomY = node->y + MARGIN;
    int maxRoomY = node->y + node->height - roomHeight - MARGIN;
    int roomY = Math::Rand(std::min(minRoomY, maxRoomY), std::max(minRoomY, maxRoomY));

    node->room = { roomX, roomY, roomWidth, roomHeight, Room::RoomType::eNormal};

    // ���̐��� (�����̓����݂̂����ɐݒ�)
    for (int y = roomY + 1; y < roomY + roomHeight - 1; y++)
    {
        for (int x = roomX + 1; x < roomX + roomWidth - 1; x++)
        {
            map[y][x].type = TileType::eFloor;
        }
    }

}

// �����̕ǃf�[�^�̐ݒ�
void CBspMap::CreateRoomWall(const Room& room, std::vector<std::vector<Tile>>& map)
{
    // �㉺�̕ǂ̐ݒ�
    for (int x = room.x + 1; x < room.x + room.width - 1; x++)
    {
        // ��̕�
        mMapData[room.y][x].type = TileType::eWall;
        mMapData[room.y][x].dir = Direction::eNorth;

        // ���̕�
        mMapData[room.y + room.height - 1][x].type = TileType::eWall;
        mMapData[room.y + room.height - 1][x].dir = Direction::eSouth;

    }
    // ���E�̕ǂ�ݒ�
    for (int y = room.y + 1; y < room.y + room.height - 1; y++)
    {
        // ���̕�
        mMapData[y][room.x].type = TileType::eWall;
        mMapData[y][room.x].dir = Direction::eWest;

        // �E�̕�
        mMapData[y][room.x + room.width - 1].type = TileType::eWall;
        mMapData[y][room.x + room.width - 1].dir = Direction::eEast;

    }
}

// �����̎l���f�[�^�̒���ݒ�
void CBspMap::CreateRoomPillar(const Room& room, std::vector<std::vector<Tile>>& map)
{
    mMapData[room.y][room.x].type = TileType::ePillar;	// ����
    mMapData[room.y][room.x].dir = Direction::eNorthWest;	// �k��
    mMapData[room.y][room.x + room.width - 1].type = TileType::ePillar;	// �E��
    mMapData[room.y][room.x + room.width - 1].dir = Direction::eNorthEast;	// �k��
    mMapData[room.y + room.height - 1][room.x].type = TileType::ePillar;	// ����
    mMapData[room.y + room.height - 1][room.x].dir = Direction::eSouthWest;	// �쐼
    mMapData[room.y + room.height - 1][room.x + room.width - 1].type = TileType::ePillar;	// �E��
    mMapData[room.y + room.height - 1][room.x + room.width - 1].dir = Direction::eSouthEast;	// �쓌
}

// �����K�w�̕������m�̒ʘH�f�[�^��ݒ�
void CBspMap::ConnectRooms(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    if (!node || !node->right || !node->left) return;

    // �ċA
    ConnectRooms(node->left, map);
    ConnectRooms(node->right, map);

    // ���̎q�̕����̒��S�_�߂����擾
    CVector2 leftRoom = GetRoomCenter(node->left);
    // �E�̎q�̕����̒��S�_�߂����擾
    CVector2 rightRoom = GetRoomCenter(node->right);

    // �ǂ��炩���q�����Ă��Ȃ����(�ʘH�̐��𐧌����邽��)
    if (!node->left->room.connected && !node->right->room.connected)
    {
        // 2�_�𒼐��Ō��Ԃ悤�ɒʘH�����
        CreatePassage(map, leftRoom, rightRoom);

        node->left->room.connected = true;
        node->right->room.connected = true;
    }

}

// �����̒����̍��W���擾
CVector2 CBspMap::GetRoomCenter(SectionNode* node)
{
    if (!node) return CVector2();

    // �t�m�[�h��T��
    if (!node->left && !node->right)
    {
        int cx = node->room.x + node->room.width / 2;
        int cy = node->room.y + node->room.height / 2;

        // ���̈�� room.x+1 .. room.x+room.width-2
        int minX = node->room.x + 1;
        int maxX = node->room.x + node->room.width - 2;
        int minY = node->room.y + 1;
        int maxY = node->room.y + node->room.height - 2;

        // �����������ď��̈悪�����ꍇ�͕ǂ̓����imin..max ���t�]����j��h��
        if (minX > maxX) { minX = maxX = node->room.x + 1; }
        if (minY > maxY) { minY = maxY = node->room.y + 1; }

        // �N���b�v
        //cx = std::max(minX, std::min(cx, maxX));
        //cy = std::max(minY, std::min(cy, maxY));
        
        cx = (cx < minX) ? minX : (cx > maxX ? maxX : cx);
        cy = (cy < minY) ? minY : (cy > maxY ? maxY : cy);

        return CVector2(cx, cy);
    }

    // �q�m�[�h��D�悵�ĒT��
    if (node->left) return GetRoomCenter(node->left);
    if (node->right) return GetRoomCenter(node->right);

    return CVector2();
}

// �����̒��S�ɋ߂����W���擾
CVector2 CBspMap::GetRoomRandomPos(SectionNode* node)
{
    if (!node) return CVector2();

    // �t�m�[�h��T��
    if (!node->left && !node->right)
    {
        int centerX = node->room.x + node->room.width / 2;
        int centerY = node->room.y + node->room.height / 2;

        // �����t�߂��ێ����₷��4�Ŋ���
        int rangeX = std::max(1, node->room.width / 4);
        int rangeY = std::max(1, node->room.height / 4);

        int cx = Math::Rand(centerX - rangeX, centerX + rangeX);
        int cy = Math::Rand(centerY - rangeY, centerY + rangeY);

        return CVector2(cx, cy);
    }

    // �q�m�[�h��D�悵�ĒT��
    if (node->left) return GetRoomRandomPos(node->left);
    if (node->right) return GetRoomRandomPos(node->right);

    return CVector2();
}

// �������m�̒ʘH�f�[�^�̐ݒ�
void CBspMap::CreatePassage(std::vector<std::vector<Tile>>& map, CVector2 a, CVector2 b)
{
    // ���W�𐮐��Ŏ擾
    int ax = a.X(), ay = a.Y();
    int bx = b.X(), by = b.Y();

    // �����_��
    // [&]�F�g�p����O���ϐ���S�ĎQ�Ɠn���ŃL���v�`������
    auto carve = [&](int x, int y, Direction dir)
    {
        if (map[y][x].type == TileType::eWall)
        {
            map[y][x].type = TileType::eEntrance;
        }
        else if (map[y][x].type == TileType::None || map[y][x].type == TileType::eBoundary)
        {
            map[y][x].type = TileType::ePassage;
            map[y][x].dir = dir;
        }
    };

    // �ʘH�J�n�����̓���1�}�X�A�I�������̓���1�}�X�ɂ��炷
    if (ax < bx) ax += 1;
    else if (ax > bx) ax -= 1;
    if (ay < by) ay += 1;
    else if (ay > by) ay -= 1;

    // ������
    int startX = std::min(ax, bx);
    int endX = std::max(ax, bx);
    for (int x = startX; x <= endX; ++x)
    {
        carve(x, ay, Direction::eEast);
    }

    // �c����
    int startY = std::min(ay, by);
    int endY = std::max(ay, by);
    for (int y = startY; y <= endY; ++y)
    {
        carve(bx, y, Direction::eNorth);
    }
}

#if _DEBUG
// ���̋��E����ݒ�
void CBspMap::DrawBoundary(SectionNode* node, std::vector<std::vector<Tile>>& map){
    if (node == nullptr) {
        return;
    }

    // �t�m�[�h�i�����������ŏ����j�Ȃ牽�����Ȃ�
    // �����ɋ��E����`�悷��ƁA�����̗̈��N�H����\�������邽��
    if (node->left == nullptr && node->right == nullptr) {
        return;
    }

    // ���E�̎q�m�[�h�����݂���ꍇ�i= �������ꂽ�m�[�h�j
    if (node->left != nullptr && node->right != nullptr) {

        // ���E�̎q�m�[�h�̍��W�E�T�C�Y����A���������𔻒肷��

        // �c�ɕ������ꂽ�ꍇ (�����قȂ�A����������)
        if (node->left->height == node->right->height && node->left->width != node->width) {

            // �c�̕�������X���W�́A�E�̎q�m�[�h�̊J�nX���W
            int boundaryX = node->right->x;

            // �������iX���W�� boundaryX �̃��C���j���ォ�牺�܂ŕ`��
            for (int y = node->y; y < node->y + node->height; y++) {
                // �}�b�v�͈̔̓`�F�b�N�i�C�ӂ������S�j
                if (y >= 0 && y < map.size() && boundaryX >= 0 && boundaryX < map[0].size()) {
                    map[y][boundaryX].type = TileType::eBoundary;
                }
            }
        }
        // ���ɕ������ꂽ�ꍇ (�������قȂ�A��������)
        else if (node->left->width == node->right->width && node->left->height != node->height) {

            // ���̕�������Y���W�́A�E�̎q�m�[�h�̊J�nY���W (node->right�͉����̋��)
            int boundaryY = node->right->y;

            // �������iY���W�� boundaryY �̃��C���j��������E�܂ŕ`��
            for (int x = node->x; x < node->x + node->width; x++) {
                // �}�b�v�͈̔̓`�F�b�N�i�C�ӂ������S�j
                if (boundaryY >= 0 && boundaryY < map.size() && x >= 0 && x < map[0].size()) {
                    map[boundaryY][x].type = TileType::eBoundary;
                }
            }
        }

        // �q�m�[�h�ɑ΂��čċA�I�ɏ����𑱂���
        DrawBoundary(node->left, map);
        DrawBoundary(node->right, map);
    }
}

// 2�����z��̃f�o�b�O�\��
void CBspMap::PrintSection()
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
            case TileType::ePassage:    printf("5"); break;
            default:                    printf("?"); break;
            }
        }
        printf("\n");
    }
}
#endif
