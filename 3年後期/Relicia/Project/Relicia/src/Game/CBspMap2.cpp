#include "CBspMap2.h"
#include "Maths.h"

// 区画の余白(区画の端から1マス空ける)
#define MARGIN 1

// 階層ごとのダンジョンの最大の広さを設定
const CBspMap2::FloorSetting CBspMap2::mFloorSettings[] =
{
    { 40, 40, 10, 7, 3 },   // 1階層
    { 50, 50, 10, 7, 4 },   // 2階層
    { 60, 60, 10, 8, 4 },   // 3階層
    { 80, 80, 12, 9, 5 },   // 4階層
};

// コンストラクタ
CBspMap2::CBspMap2(int floorNum)
    : mMinSectionSize(0)
    , mMapWidth(0)
    , mMapHeight(0)
    , mMinRoomSize(0)
    , mMaxDepth(0)
{
    // 階層の範囲外参照を防止
    int maxFloor = sizeof(mFloorSettings) / sizeof(mFloorSettings[0]);
    floorNum = Math::Clamp(floorNum, 0, maxFloor);

    // 階層に応じた初期化
    InitializeForFloor(floorNum);

    // 再帰的に区画を分割
    Split(mpRoot, 0);
    // 床の生成
    BuildRoom(mpRoot);
    // 通路の生成
    BuildCorridor(mpRoot);
    // 壁の生成
    BuildWalls();

#if _DEBUG
    PrintSection(mpRoot);

    DebugPrintDungion();
#endif
}

// デストラクタ
CBspMap2::~CBspMap2()
{
    DeleteSection(mpRoot);
    mpRoot = nullptr;
}

// 階層に応じた初期化
void CBspMap2::InitializeForFloor(int floorNum)
{
    const FloorSetting& fs = mFloorSettings[floorNum];

    mMinSectionSize = fs.minSectionSize;
    mMinRoomSize = fs.minRoomSize;
    mMaxDepth = fs.maxDepth;

    Initialize(fs.mapWidth, fs.mapHeight);
}

// 2次元配列のマップデータの取得
const std::vector<std::vector<CBspMap2::Tile>>& CBspMap2::GetMapData() const
{
    return mMapData;
}

// 初期化
void CBspMap2::Initialize(int width, int height)
{
    mMapWidth = width;
    mMapHeight = height;

    mMapData.resize(height, std::vector<Tile>(width));

    mpRoot = new SectionNode();
    mpRoot->x = 0;
    mpRoot->y = 0;
    mpRoot->width = width;
    mpRoot->height = height;
}

// ノードの削除
void CBspMap2::DeleteSection(SectionNode* node)
{
    if (!node) return;
    // 再帰
    DeleteSection(node->left);
    DeleteSection(node->right);
    delete node;
}

// 区画分け
void CBspMap2::Split(SectionNode* node, int depth)
{
    if (!node) return;

    if (depth >= mMaxDepth) return;

    // 横または縦が設定しているサイズ以下なら分割しない
    if (node->width < mMinSectionSize * 2 || node->height < mMinSectionSize * 2)
        return;

    // 区画を分ける方向を決める
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

    // trueなら縦に分割
    if (splitVertical && node->width >= mMinSectionSize * 2)
    {
        int splitX = Math::Rand(node->x + mMinSectionSize, node->x + node->width - mMinSectionSize);
        int left = splitX - node->x;
        int right = node->width - left;

        node->left = new SectionNode{ node->x, node->y, left, node->height, nullptr, nullptr, {} };
        node->right = new SectionNode{ splitX, node->y, right, node->height, nullptr, nullptr, {} };

    }
    // falseなら横に分割
    else if (!splitVertical && node->height >= mMinSectionSize * 2)
    {
        int splitY = Math::Rand(node->y + mMinSectionSize, node->y + node->height - mMinSectionSize);
        int top = splitY - node->y;
        int bottom = node->height - top;

        node->left = new SectionNode{ node->x, node->y, node->width, top, nullptr, nullptr, {} };
        node->right = new SectionNode{ node->x, splitY, node->width, bottom, nullptr, nullptr, {} };

    }

    // 分割がされていたら
    if (node->left != nullptr && node->right != nullptr)
    {
        // 再帰的に分割
        Split(node->left, depth + 1);
        Split(node->right,depth + 1);
    }
}

// 部屋の床を構築
void CBspMap2::BuildRoom(SectionNode* node)
{
    if (!node) return;

    if (node->left || node->right)
    {
        // 再帰
        BuildRoom(node->left);
        BuildRoom(node->right);
        return;
    }

    // サイズチェック
    if (node->width < mMinRoomSize + MARGIN * 2 ||
        node->height < mMinRoomSize + MARGIN * 2)
        return;

    int maxW = node->width - MARGIN * 2;
    int maxH = node->height - MARGIN * 2;

    int w = Math::Rand(mMinRoomSize, maxW);
    int h = Math::Rand(mMinRoomSize, maxH);

    int x = Math::Rand(
        node->x + MARGIN,
        node->x + node->width - w - MARGIN
    );
    int y = Math::Rand(
        node->y + MARGIN,
        node->y + node->height - h - MARGIN
    );

    Room room;
    room.x = x;
    room.y = y;
    room.w = w;
    room.h = h;
    // 中心座標を求める
    room.center = CVector2(x + w * 0.5f, y + h * 0.5f);

    node->room = room;
    node->connectPoint = room.center;

    // 床を設定
    for (int iy = y; iy < y + h; iy++)
    {
        for (int ix = x; ix < x + w; ix++)
        {
            mMapData[iy][ix].type = TileType::eFloor;
        }
    }
}

// 壁の構築
void CBspMap2::BuildWalls()
{
    for (int y = 0; y < mMapHeight; y++)
    {
        for (int x = 0; x < mMapWidth; x++)
        {
            if (mMapData[y][x].type != TileType::eFloor)
                continue;

            // 北
            if (y == 0 || mMapData[y - 1][x].type == TileType::None)
                mMapData[y][x].north = EdgeType::eWall;

            // 南
            if (y == mMapHeight - 1 || mMapData[y + 1][x].type == TileType::None)
                mMapData[y][x].south = EdgeType::eWall;

            // 西
            if (x == 0 || mMapData[y][x - 1].type == TileType::None)
                mMapData[y][x].west = EdgeType::eWall;

            // 東
            if (x == mMapWidth - 1 || mMapData[y][x + 1].type == TileType::None)
                mMapData[y][x].east = EdgeType::eWall;
        }
    }
}

// 通路の構築
void CBspMap2::BuildCorridor(SectionNode* node)
{
    if (!node || !node->left || !node->right)
        return;

    // 再帰
    BuildCorridor(node->left);
    BuildCorridor(node->right);

    CVector2 p1 = node->left->connectPoint;
    CVector2 p2 = node->right->connectPoint;

    // 通路を掘る
    DigCorridor(p1, p2);

    node->connectPoint = (p1 + p2) * 0.5f;
}

// 通路を掘る
void CBspMap2::DigCorridor(CVector2 from, CVector2 to)
{
    int x1 = (int)from.X();
    int y1 = (int)from.Y();
    int x2 = (int)to.X();
    int y2 = (int)to.Y();

    // 横 → 縦 or 縦 → 横
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); x++)
        mMapData[y1][x].type = TileType::eFloor;

    // 縦 → 横 or 横 → 縦
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++)
        mMapData[y][x2].type = TileType::eFloor;
}

#if _DEBUG 
// 区画の表示
void CBspMap2::PrintSection(const SectionNode* node, int depth)
{
    if (!node) return;

    //for (int i = 0; i < depth; i++) printf("  ");
    //printf("Section x=%d y=%d w=%d h=%d\n",
    //    node->x, node->y, node->width, node->height);

    if (!node->left && !node->right)
    {
        printf("[Depth %d] Leaf Room: x=%d y=%d w=%d h=%d\n",
            depth, node->x, node->y, node->width, node->height);
    }

    PrintSection(node->left, depth + 1);
    PrintSection(node->right, depth + 1);
}

// 部屋の床の表示
void CBspMap2::DebugPrintFloor() const
{
    for (int y = 0; y < mMapHeight; y++)
    {
        for (int x = 0; x < mMapWidth; x++)
        {
            if (mMapData[y][x].type == TileType::eFloor)
                printf("1");    // 床
            else
                printf("0");    // 無し
        }
        printf("\n");
    }
}

// ダンジョンの表示
void CBspMap2::DebugPrintDungion() const
{
    for (int y = 0; y < mMapHeight; y++)
    {
        for (int x = 0; x < mMapWidth; x++)
        {
            const Tile& t = mMapData[y][x];

            if (t.type != TileType::eFloor)
            {
                printf("x");     // 何もない
            }
            else if ( t.north == EdgeType::eWall ||
                      t.south == EdgeType::eWall ||
                      t.east ==  EdgeType::eWall ||
                      t.west ==  EdgeType::eWall)
            {
                printf("#");    // 壁ありの床
            }
            else
            {
                printf(" ");    // 壁なしの床
            }
        }
        printf("\n");
    }
}
#endif
