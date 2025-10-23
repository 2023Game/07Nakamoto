#include "CBpsMap.h"
#include "Maths.h"

// 区画の最小サイズ
#define MIN_SIZE 40
// 部屋の最小サイズ
#define MIN_ROOM 5

// 区画の余白(区画の端から1マス空ける)
#define MARGIN 1

// コンストラクタ
CBpsMap::CBpsMap(int x, int y)
{
    // 初期化
    Initialize(x, y);

    // 再帰的に区画を分割
    Split(mpRoot);

    // 各区画に部屋を配置
    PlaceRoom(mpRoot, mMapData);

#if _DEBUG
    // ２次元配列のデバッグ表示
    PrintSection();
#endif
}

// デストラクタ
CBpsMap::~CBpsMap()
{
    DeleteNode(mpRoot);
}

// ノードの削除
void CBpsMap::DeleteNode(SectionNode* node)
{
    if (node == nullptr)
        return;
    DeleteNode(node->left);
    DeleteNode(node->right);
    delete node;
}

// 初期化
void CBpsMap::Initialize(int width, int height)
{
    mMapData.resize(height, std::vector<Room>(width, Room{ 0,0,0,0, CBpsMap::TileType::None }));

    // 最初の大きな区画（全体）
    mpRoot = new SectionNode();
    mpRoot->x = 0;
    mpRoot->y = 0;
    mpRoot->width = width;
    mpRoot->height = height;
    mpRoot->left = nullptr;
    mpRoot->right = nullptr;
}

// 区画分け
void CBpsMap::Split(SectionNode* node)
{
    // 設定しているサイズ以下なら分割しない
    if (node->width < MIN_SIZE && node->height < MIN_SIZE)
        return;

    bool splitVertical = (rand() % 2 == 0);

    if (splitVertical && node->width >= MIN_SIZE)
    {
        int splitX = Math::Rand(node->x + MIN_SIZE, node->x + node->width - MIN_SIZE);
        int left = splitX - node->x;
        int right = node->width - left;

        //if (left > right) return; // 分割できないので終了

        node->left = new SectionNode{ node->x, node->y, left, node->height, nullptr, nullptr };
        node->right = new SectionNode{ splitX, node->y, right, node->height, nullptr, nullptr };
    }
    else if (!splitVertical && node->height >= MIN_SIZE)
    {
        int splitY = Math::Rand(node->y + MIN_SIZE, node->y + node->height - MIN_SIZE);
        int top = splitY - node->y;
        int bottom = node->height - top;

        //if (top > bottom) return; // 分割できないので終了

        node->left = new SectionNode{ node->x, node->y, node->width, top, nullptr, nullptr };
        node->right = new SectionNode{ node->x, splitY, node->width, bottom, nullptr, nullptr };
    }
    else
    {
        return; // どちらも分割できない
    }

    // 再帰的に分割
    Split(node->left);
    Split(node->right);
}

// 部屋の設定
void CBpsMap::PlaceRoom(SectionNode* node, std::vector<std::vector<Room>>& map)
{
    if (node == nullptr)
        return;

    // 子ノードが無ければ（=葉ノード）部屋を作る
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
                map[y][x].type = TileType::eFloor; // 床にする
    }
    else
    {
        // 子ノードにも部屋を作る
        PlaceRoom(node->left, map);
        PlaceRoom(node->right, map);
    }
}

#if _DEBUG
// 2次元配列のデバッグ表示
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
