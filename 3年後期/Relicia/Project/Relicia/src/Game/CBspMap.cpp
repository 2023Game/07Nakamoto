#include "CBspMap.h"
#include "Maths.h"

// 区画の最小サイズ
#define MIN_SIZE 10
// 部屋の最小サイズ
#define MIN_ROOM 5

// 区画の余白(区画の端から1マス空ける)
#define MARGIN 1

// コンストラクタ
CBspMap::CBspMap(int x, int y)
{
    // 初期化
    Initialize(x, y);

    // 再帰的に区画を分割
    Split(mpRoot);

#if _DEBUG
    // 区画の境界線設定(境界線をみたい場合のみ使用)
    DrawBoundary(mpRoot, mMapData);
#endif

    // 各区画に部屋を配置
    PlaceRoom(mpRoot, mMapData);
    // 同じ階層の部屋同士を通路で繋げる
    ConnectRooms(mpRoot, mMapData);

#if _DEBUG
    // ２次元配列のデバッグ表示
    PrintSection();
#endif
}

// デストラクタ
CBspMap::~CBspMap()
{
    DeleteNode(mpRoot);
}


// BPSマップデータの取得
const std::vector<std::vector<CBspMap::Tile>>& CBspMap::GetTileData() const
{
    return mMapData;
}

// ノードの削除
void CBspMap::DeleteNode(SectionNode* node)
{
    if (node == nullptr) return;

    DeleteNode(node->left);
    DeleteNode(node->right);
    delete node;
}

// 初期化
void CBspMap::Initialize(int width, int height)
{
    mMapData.resize(height, std::vector<Tile>(width));

    // タイル情報の初期化
    for (auto& row : mMapData)
    {
        for (auto& tile : row)
        {
            tile = { TileType::None,Direction::eNorth };
        }
    }

    // 最初の大きな区画の初期化（全体）
    mpRoot = new SectionNode();
    mpRoot->x = 0;
    mpRoot->y = 0;
    mpRoot->width = width;
    mpRoot->height = height;
    mpRoot->left = nullptr;
    mpRoot->right = nullptr;
}

// 区画分け
void CBspMap::Split(SectionNode* node)
{
    // 横または縦が設定しているサイズ以下なら分割しない
    if (node->width < MIN_SIZE * 2 || node->height < MIN_SIZE * 2)
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
    // falseなら横に分割
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

    // 分割がされていたら
    if (node->left != nullptr && node->right != nullptr)
    {
        // 再帰的に分割
        Split(node->left);
        Split(node->right);
    }
}

// 部屋の設定
void CBspMap::PlaceRoom(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    if (node == nullptr) return;

    // 子ノードが無ければ（=葉ノード）部屋を作る
    if (node->left == nullptr && node->right == nullptr)
    {
        // 部屋の最小サイズ + 両側のマージン < 区画サイズ のチェック
        // このチェックで、以降の Math::Rand で引数が逆転する可能性を大幅に減らせる
        if (node->width < MIN_ROOM + MARGIN * 2 || node->height < MIN_ROOM + MARGIN * 2)
        {
            // 区画が小さすぎるため、部屋を作らずに終了
            return;
        }

        // 部屋の床データの設定
        CreateRoomFloor(node, map);
        // 部屋の壁データの設定
        CreateRoomWall(node->room, map);
        // 部屋の四隅の柱データの設定
        CreateRoomPillar(node->room, map);
    }
    else
    {
        // 子ノードにも部屋を作る
        PlaceRoom(node->left, map);
        PlaceRoom(node->right, map);
    }
}

// 部屋の床データの設定
void CBspMap::CreateRoomFloor(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    // 1. 部屋のサイズを決定 (最大値は node->size - 2*MARGIN)
    int maxRoomWidth = node->width - (MARGIN * 2);
    int roomWidth = Math::Rand(MIN_ROOM, maxRoomWidth);

    int maxRoomHeight = node->height - (MARGIN * 2);
    int roomHeight = Math::Rand(MIN_ROOM, maxRoomHeight);

    if (maxRoomWidth < MIN_ROOM || maxRoomHeight < MIN_ROOM)
        return;

    // 2. 部屋の配置座標を決定 (Math::Randの引数の順序を保証)

    // X座標
    int minRoomX = node->x + MARGIN;
    int maxRoomX = node->x + node->width - roomWidth - MARGIN;
    int roomX = Math::Rand(std::min(minRoomX, maxRoomX), std::max(minRoomX, maxRoomX));

    // Y座標
    int minRoomY = node->y + MARGIN;
    int maxRoomY = node->y + node->height - roomHeight - MARGIN;
    int roomY = Math::Rand(std::min(minRoomY, maxRoomY), std::max(minRoomY, maxRoomY));

    node->room = { roomX, roomY, roomWidth, roomHeight, Room::RoomType::eNormal};

    // 床の生成 (部屋の内側のみを床に設定)
    for (int y = roomY + 1; y < roomY + roomHeight - 1; y++)
    {
        for (int x = roomX + 1; x < roomX + roomWidth - 1; x++)
        {
            map[y][x].type = TileType::eFloor;
        }
    }

}

// 部屋の壁データの設定
void CBspMap::CreateRoomWall(const Room& room, std::vector<std::vector<Tile>>& map)
{
    // 上下の壁の設定
    for (int x = room.x + 1; x < room.x + room.width - 1; x++)
    {
        // 上の壁
        mMapData[room.y][x].type = TileType::eWall;
        mMapData[room.y][x].dir = Direction::eNorth;

        // 下の壁
        mMapData[room.y + room.height - 1][x].type = TileType::eWall;
        mMapData[room.y + room.height - 1][x].dir = Direction::eSouth;

    }
    // 左右の壁を設定
    for (int y = room.y + 1; y < room.y + room.height - 1; y++)
    {
        // 左の壁
        mMapData[y][room.x].type = TileType::eWall;
        mMapData[y][room.x].dir = Direction::eWest;

        // 右の壁
        mMapData[y][room.x + room.width - 1].type = TileType::eWall;
        mMapData[y][room.x + room.width - 1].dir = Direction::eEast;

    }
}

// 部屋の四隅データの柱を設定
void CBspMap::CreateRoomPillar(const Room& room, std::vector<std::vector<Tile>>& map)
{
    mMapData[room.y][room.x].type = TileType::ePillar;	// 左上
    mMapData[room.y][room.x].dir = Direction::eNorthWest;	// 北西
    mMapData[room.y][room.x + room.width - 1].type = TileType::ePillar;	// 右上
    mMapData[room.y][room.x + room.width - 1].dir = Direction::eNorthEast;	// 北東
    mMapData[room.y + room.height - 1][room.x].type = TileType::ePillar;	// 左下
    mMapData[room.y + room.height - 1][room.x].dir = Direction::eSouthWest;	// 南西
    mMapData[room.y + room.height - 1][room.x + room.width - 1].type = TileType::ePillar;	// 右下
    mMapData[room.y + room.height - 1][room.x + room.width - 1].dir = Direction::eSouthEast;	// 南東
}

// 同じ階層の部屋同士の通路データを設定
void CBspMap::ConnectRooms(SectionNode* node, std::vector<std::vector<Tile>>& map)
{
    if (!node || !node->right || !node->left) return;

    // 再帰
    ConnectRooms(node->left, map);
    ConnectRooms(node->right, map);

    // 左の子の部屋の中心点近くを取得
    CVector2 leftRoom = GetRoomCenter(node->left);
    // 右の子の部屋の中心点近くを取得
    CVector2 rightRoom = GetRoomCenter(node->right);

    // どちらかが繋がっていなければ(通路の数を制限するため)
    if (!node->left->room.connected && !node->right->room.connected)
    {
        // 2点を直線で結ぶように通路を作る
        CreatePassage(map, leftRoom, rightRoom);

        node->left->room.connected = true;
        node->right->room.connected = true;
    }

}

// 部屋の中央の座標を取得
CVector2 CBspMap::GetRoomCenter(SectionNode* node)
{
    if (!node) return CVector2();

    // 葉ノードを探す
    if (!node->left && !node->right)
    {
        int cx = node->room.x + node->room.width / 2;
        int cy = node->room.y + node->room.height / 2;

        // 床領域は room.x+1 .. room.x+room.width-2
        int minX = node->room.x + 1;
        int maxX = node->room.x + node->room.width - 2;
        int minY = node->room.y + 1;
        int maxY = node->room.y + node->room.height - 2;

        // 幅が小さくて床領域が無い場合は壁の内側（min..max が逆転する）を防ぐ
        if (minX > maxX) { minX = maxX = node->room.x + 1; }
        if (minY > maxY) { minY = maxY = node->room.y + 1; }

        // クリップ
        //cx = std::max(minX, std::min(cx, maxX));
        //cy = std::max(minY, std::min(cy, maxY));
        
        cx = (cx < minX) ? minX : (cx > maxX ? maxX : cx);
        cy = (cy < minY) ? minY : (cy > maxY ? maxY : cy);

        return CVector2(cx, cy);
    }

    // 子ノードを優先して探す
    if (node->left) return GetRoomCenter(node->left);
    if (node->right) return GetRoomCenter(node->right);

    return CVector2();
}

// 部屋の中心に近い座標を取得
CVector2 CBspMap::GetRoomRandomPos(SectionNode* node)
{
    if (!node) return CVector2();

    // 葉ノードを探す
    if (!node->left && !node->right)
    {
        int centerX = node->room.x + node->room.width / 2;
        int centerY = node->room.y + node->room.height / 2;

        // 中央付近を維持しやすい4で割る
        int rangeX = std::max(1, node->room.width / 4);
        int rangeY = std::max(1, node->room.height / 4);

        int cx = Math::Rand(centerX - rangeX, centerX + rangeX);
        int cy = Math::Rand(centerY - rangeY, centerY + rangeY);

        return CVector2(cx, cy);
    }

    // 子ノードを優先して探す
    if (node->left) return GetRoomRandomPos(node->left);
    if (node->right) return GetRoomRandomPos(node->right);

    return CVector2();
}

// 部屋同士の通路データの設定
void CBspMap::CreatePassage(std::vector<std::vector<Tile>>& map, CVector2 a, CVector2 b)
{
    // 座標を整数で取得
    int ax = a.X(), ay = a.Y();
    int bx = b.X(), by = b.Y();

    // ラムダ式
    // [&]：使用する外部変数を全て参照渡しでキャプチャする
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

    // 通路開始を床の内側1マス、終了も床の内側1マスにずらす
    if (ax < bx) ax += 1;
    else if (ax > bx) ax -= 1;
    if (ay < by) ay += 1;
    else if (ay > by) ay -= 1;

    // 横方向
    int startX = std::min(ax, bx);
    int endX = std::max(ax, bx);
    for (int x = startX; x <= endX; ++x)
    {
        carve(x, ay, Direction::eEast);
    }

    // 縦方向
    int startY = std::min(ay, by);
    int endY = std::max(ay, by);
    for (int y = startY; y <= endY; ++y)
    {
        carve(bx, y, Direction::eNorth);
    }
}

#if _DEBUG
// 区画の境界線を設定
void CBspMap::DrawBoundary(SectionNode* node, std::vector<std::vector<Tile>>& map){
    if (node == nullptr) {
        return;
    }

    // 葉ノード（部屋が作られる最小区画）なら何もしない
    // ここに境界線を描画すると、部屋の領域を侵食する可能性があるため
    if (node->left == nullptr && node->right == nullptr) {
        return;
    }

    // 左右の子ノードが存在する場合（= 分割されたノード）
    if (node->left != nullptr && node->right != nullptr) {

        // 左右の子ノードの座標・サイズから、分割方向を判定する

        // 縦に分割された場合 (幅が異なり、高さが同じ)
        if (node->left->height == node->right->height && node->left->width != node->width) {

            // 縦の分割線のX座標は、右の子ノードの開始X座標
            int boundaryX = node->right->x;

            // 分割線（X座標が boundaryX のライン）を上から下まで描画
            for (int y = node->y; y < node->y + node->height; y++) {
                // マップの範囲チェック（任意だが安全）
                if (y >= 0 && y < map.size() && boundaryX >= 0 && boundaryX < map[0].size()) {
                    map[y][boundaryX].type = TileType::eBoundary;
                }
            }
        }
        // 横に分割された場合 (高さが異なり、幅が同じ)
        else if (node->left->width == node->right->width && node->left->height != node->height) {

            // 横の分割線のY座標は、右の子ノードの開始Y座標 (node->rightは下側の区画)
            int boundaryY = node->right->y;

            // 分割線（Y座標が boundaryY のライン）を左から右まで描画
            for (int x = node->x; x < node->x + node->width; x++) {
                // マップの範囲チェック（任意だが安全）
                if (boundaryY >= 0 && boundaryY < map.size() && x >= 0 && x < map[0].size()) {
                    map[boundaryY][x].type = TileType::eBoundary;
                }
            }
        }

        // 子ノードに対して再帰的に処理を続ける
        DrawBoundary(node->left, map);
        DrawBoundary(node->right, map);
    }
}

// 2次元配列のデバッグ表示
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
