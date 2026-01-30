#include "CBspMap.h"
#include "CBspMapCollider.h"
#include "Maths.h"

// 区画の最小サイズ
#define MIN_SIZE 10
// 部屋の最小サイズ
#define MIN_ROOM 7

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
    
    // 通路のまとまりを保存
    //SetPassageData();

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

// ルートノードの取得
const CBspMap::SectionNode* CBspMap::GetRootNode() const
{
    return mpRoot;
}

// 通路と部屋の壁の開始座標と終了座標のリストを取得
std::vector<CBspMap::TileSegment> CBspMap::GetWallSegments() const
{
    std::vector<CBspMap::TileSegment> walls;

    // 横方向の走査
    for (size_t y = 0; y < mMapData.size(); ++y)
    {
        int iy = static_cast<int>(y);

        int xStart = -1;
        CBspMap::Direction dir = CBspMap::Direction::None;

        int startUpWallX = -1;
        int startDownWallX = -1;

        for (int x = 0; x < mMapData[y].size(); ++x)
        {
            int ix = static_cast<int>(x);

            //// 横方向の通路かどうか
            //bool isPassage = mMapData[y][x].passage &&
            //    mMapData[y][x].passageData.dir == Direction::eEast;

            //// 横方向かつ開始座標かどうか
            //if (isPassage && startX == -1)
            //{
            //    startX = x;
            //}
            //// まだ通路かどうか
            //else if (!isPassage && startX != -1)
            //{
            //    // 通路の床のコライダー情報を登録
            //    floors.push_back({
            //        CVector2(startX,y), CVector2(x,y),
            //        TileType::ePassage, Direction::eEast
            //        });

            //    startX = -1;
            //}

            // 上下の部屋の壁かどうか
            bool isWall = ((mMapData[iy][ix].dir == Direction::eNorth ||
                mMapData[iy][ix].dir == Direction::eSouth) &&
                mMapData[iy][ix].type == TileType::eWall);

            // 始めの壁だったら
            if (isWall && xStart == -1)
            {
                xStart = static_cast<int>(ix);
                dir = mMapData[iy][ix].dir;
            }
            else if (!isWall && xStart != -1 && ix > 0)
            {
                if (mMapData[iy][ix - 1].dir == Direction::eNorth)
                {
                    walls.push_back({
                        CVector2(static_cast<float>(xStart), static_cast<float>(iy) + 1.0f),
                        CVector2(static_cast<float>(ix),static_cast<float>(iy + 1)),
                        TileType::eWall, dir });
                }
                else if (mMapData[y][ix - 1].dir == Direction::eSouth)
                {
                    walls.push_back({
                        CVector2(static_cast<float>(xStart), static_cast<float>(iy) - 1.0f),
                        CVector2(static_cast<float>(ix),   static_cast<float>(iy) - 1.0f),
                        TileType::eWall, dir });
                }
                xStart = -1;
            }

            // 通路の北方向に壁があるか
            bool upWall = 
                iy > 0 &&
                mMapData[iy][ix].passage && 
                !mMapData[iy - 1][ix].passage &&
                (mMapData[iy - 1][ix].type != TileType::eFloor);

            // 北方向の壁の開始位置かどうか
            if (upWall && startUpWallX == -1)
            {
                startUpWallX = ix;
            }
            // 北方向の壁の終了位置かどうか
            else if (!upWall && startUpWallX != -1)
            {
                // 通路の壁のコライダー情報を登録
                walls.push_back({
                    CVector2(static_cast<float>(startUpWallX),static_cast<float>(iy)), 
                    CVector2(static_cast<float>(x),static_cast<float>(iy)),
                    TileType::eWall, Direction::eNorth });

                startUpWallX = -1;
            }

            // 通路の南方向に壁があるか
            bool downWall = 
                iy + 1 < mMapData.size() && 
                mMapData[iy][ix].passage &&
                !mMapData[iy + 1][ix].passage &&
                (mMapData[iy + 1][ix].type != TileType::eFloor);

            // 南方向の壁の開始位置かどうか
            if (downWall && startDownWallX == -1)
            {
                startDownWallX = ix;
            }
            // 南方向の壁の終了位置かどうか
            else if (!downWall && startDownWallX != -1)
            {
                // 通路の壁のコライダー情報を登録
                walls.push_back({
                    CVector2(static_cast<float>(startDownWallX),static_cast<float>(iy)),
                    CVector2(static_cast<float>(ix),static_cast<float>(iy)),
                    TileType::eWall, Direction::eSouth });

                startDownWallX = -1;
            }
        }

        //// 行の最後まで通路が続いていた場合
        //if (startX != -1)
        //{
        //    // 通路の床のコライダー情報を登録
        //    floors.push_back({
        //        CVector2(startX, y), CVector2((int)mMapData[y].size() - 1, y),
        //        TileType::ePassage, Direction::eEast
        //        });

        //    startX = -1;
        //}
        
        float endX = static_cast<float>(mMapData[iy].size() - 1);

        // 行の最後まで上の壁が続いていた場合
        if (startUpWallX != -1)
        {
            // 通路の壁のコライダー情報を登録
            walls.push_back({
            CVector2(static_cast<float>(startUpWallX),static_cast<float>(iy)), 
            CVector2(endX,static_cast<float>(iy)),
            TileType::eWall, Direction::eNorth });

            startUpWallX = -1;
        }
        // 行の最後まで下の壁が続いていた場合
        if (startDownWallX != -1)
        {
            // 通路の壁のコライダー情報を登録
            walls.push_back({
            CVector2(static_cast<float>(startDownWallX),static_cast<float>(iy)), 
            CVector2(endX,static_cast<float>(iy)),
            TileType::eWall, Direction::eSouth });

            startDownWallX = -1;
        }
    }

    // 縦方向の走査
    for (size_t x = 0; x < mMapData[0].size(); ++x)
    {
        int ix = static_cast<int>(x);

        int yStart = -1;
        CBspMap::Direction dir = CBspMap::Direction::None;

        int startLeftWallY = -1;
        int startRightWallY = -1;

        for (size_t y = 0; y < mMapData.size(); ++y)
        {
            int iy = static_cast<int>(y);

            //// 縦方向の通路かどうか
            //bool isPassage = (mMapData[y][x].passage &&
            //    mMapData[y][x].passageData.dir == Direction::eNorth);

            //// 縦方向かつ開始座標かどうか
            //if (isPassage && startY == -1)
            //{
            //    startY = y;
            //}
            //// まだ通路かどうか
            //else if (!isPassage && startY != -1)
            //{
            //    // 通路の床のコライダー情報を登録
            //    floors.push_back({
            //        CVector2(x,startY), CVector2(x,y),
            //        TileType::ePassage, Direction::eNorth
            //        });

            //    startY = -1;
            //}

            // 左右の部屋の壁かどうか
            bool isWall = ((mMapData[iy][ix].dir == Direction::eEast ||
                mMapData[iy][ix].dir == Direction::eWest) &&
                mMapData[iy][ix].type == TileType::eWall);

            if (isWall && yStart == -1)
            {
                yStart = static_cast<int>(y);
                dir = mMapData[iy][ix].dir;
            }
            else if (!isWall && yStart != -1 && iy > 0)
            {
                if (mMapData[iy - 1][ix].dir == Direction::eEast)
                {
                    walls.push_back({
                        CVector2(static_cast<float>(ix) - 1.0f, static_cast<float>(yStart)),
                        CVector2(static_cast<float>(ix) - 1.0f, static_cast<float>(iy)),
                        TileType::eWall, dir });
                }
                else if (mMapData[iy - 1][ix].dir == Direction::eWest)
                {
                    walls.push_back({
                        CVector2(static_cast<float>(ix) + 1.0f, static_cast<float>(yStart)),
                        CVector2(static_cast<float>(ix) + 1.0f, static_cast<float>(iy)),
                        TileType::eWall, dir });
                }
                yStart = -1;
            }


            // 通路の西方向に壁があるか
            bool leftWall = 
                ix > 0 &&
                mMapData[iy][ix].passage &&
                !mMapData[iy][ix - 1].passage &&
                (mMapData[iy][ix - 1].type != TileType::eFloor || 
                 mMapData[iy][ix].type == TileType::eWall);

            // 西方向の壁の開始位置かどうか
            if (leftWall && startLeftWallY == -1)
            {
                startLeftWallY = iy;
            }
            // 西方向の壁の終了位置かどうか
            else if (!leftWall && startLeftWallY != -1)
            {
                // 通路の壁のコライダー情報を登録
                walls.push_back({
                    CVector2(static_cast<float>(ix),static_cast<float>(startLeftWallY)), 
                    CVector2(static_cast<float>(ix),static_cast<float>(iy)),
                    TileType::eWall, Direction::eWest });

                startLeftWallY = -1;
            }

            // 通路の東方向に壁があるか
            bool rightWall = 
                ix + 1 < mMapData[iy].size() &&
                mMapData[iy][ix].passage && 
                !mMapData[iy][ix + 1].passage &&
                (mMapData[iy][ix + 1].type != TileType::eFloor || mMapData[iy][ix].type == TileType::eWall);

            // 東方向の壁の開始位置かどうか
            if (rightWall && startRightWallY == -1)
            {
                startRightWallY = iy;
            }
            // 東方向の壁の終了位置かどうか
            else if (!rightWall && startRightWallY != -1)
            {
                // 通路の壁のコライダー情報を登録
                walls.push_back({
                    CVector2(static_cast<float>(ix),static_cast<float>(startRightWallY)), 
                    CVector2(static_cast<float>(ix),static_cast<float>(iy)),
                    TileType::eWall, Direction::eEast });

                startRightWallY = -1;
            }

        }
        //// 行の最後まで通路が続いていた場合
        //if (startY != -1)
        //{
        //    // 通路の床のコライダー情報を登録
        //    floors.push_back({
        //        CVector2(x, startY), CVector2((int)mMapData.size() - 1, startY),
        //        TileType::ePassage, Direction::eNorth
        //        });

        //    startY = -1;
        //}

        float endY = static_cast<float>(mMapData.size() - 1);
        
        // 行の最後まで左の壁が続いていた場合
        if (startLeftWallY != -1)
        {
            // 通路の壁のコライダー情報を登録
            walls.push_back({
            CVector2(static_cast<float>(ix),static_cast<float>(startLeftWallY)), 
            CVector2(static_cast<float>(ix), static_cast<float>(endY)),
            TileType::eWall, Direction::eWest });

            startLeftWallY = -1;
        }
        // 行の最後まで右の壁が続いていた場合
        if (startRightWallY != -1)
        {
            // 通路の壁のコライダー情報を登録
            walls.push_back({
            CVector2(static_cast<float>(ix),static_cast<float>(startRightWallY)),
            CVector2(static_cast<float>(ix), static_cast<float>(endY)),
            TileType::eWall, Direction::eEast });

            startRightWallY = -1;
        }
    }

    return walls;
}

// 部屋の壁の情報を返す
//std::vector<CBspMap::TileSegment> CBspMap::CollectWallSegments() const
//{
//    std::vector<TileSegment> walls;
//
//    // タイル走査して壁の連続範囲を検出
//    // 左から右へ走査して連続する壁をまとめる
//    for (size_t y = 0; y < mMapData.size(); ++y)
//    {
//        int xStart = -1;
//        CBspMap::Direction dir = CBspMap::Direction::None;
//
//        for (size_t x = 0; x < mMapData[y].size(); ++x)
//        {
//            // 上下の壁かどうか
//            bool isWall = ((mMapData[y][x].dir == Direction::eNorth ||
//                            mMapData[y][x].dir == Direction::eSouth) &&
//                            mMapData[y][x].type == TileType::eWall);
//
//            // 始めの壁だったら
//            if (isWall && xStart == -1)
//            {
//                xStart = (int)x;
//                dir =mMapData[y][x].dir;
//
//            }
//            else if (!isWall && xStart != -1 )
//            {
//                if (mMapData[y][x - 1].dir == Direction::eNorth)
//                {
//                    walls.push_back({
//                        CVector2((float)xStart, (float)y + 1),
//                        CVector2((float)x,   (float)y + 1),
//                        TileType::eWall, dir });
//                }
//                else if (mMapData[y][x - 1].dir == Direction::eSouth)
//                {
//                    walls.push_back({
//                        CVector2((float)xStart, (float)y - 1),
//                        CVector2((float)x,   (float)y - 1),
//                        TileType::eWall, dir });
//                }
//                xStart = -1;
//            }
//        }
//    }
//
//    // 上から下へ走査して連続する壁をまとめる
//    for (size_t x = 0; x < mMapData[0].size(); ++x)
//    {
//        int yStart = -1;
//        CBspMap::Direction dir = CBspMap::Direction::None;
//
//        for (size_t y = 0; y < mMapData.size(); ++y)
//        {
//            // 左右の壁かどうか
//            bool isWall = ((mMapData[y][x].dir == Direction::eEast ||
//                            mMapData[y][x].dir == Direction::eWest) &&
//                            mMapData[y][x].type == TileType::eWall);
//
//            if (isWall && yStart == -1)
//            {
//                yStart = (int)y;
//                dir =mMapData[y][x].dir;
//            }
//            else if (!isWall && yStart != -1)
//            {
//                if (mMapData[y - 1][x].dir == Direction::eEast)
//                {
//                    walls.push_back({
//                        CVector2((float)x - 1, (float)yStart),
//                        CVector2((float)x - 1, (float)y),
//                        TileType::eWall, dir });
//                }
//                else if (mMapData[y - 1][x].dir == Direction::eWest)
//                {
//                    walls.push_back({ 
//                        CVector2((float)x + 1, (float)yStart),
//                        CVector2((float)x + 1, (float)y),
//                        TileType::eWall, dir });
//                }
//                yStart = -1;
//            }
//        }
//    }
//    return walls;
//}

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
    // タイル情報の初期化
    mMapData.resize(height, std::vector<Tile>(width));

    //for (auto& row : mMapData)
    //{
    //    for (auto& tile : row)
    //    {
    //        tile = { TileType::None, Direction::eNorth, false};
    //    }
    //}

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

    // 部屋の中心座標
    float centerX = roomX + roomWidth * 0.5f;
    float centerY = roomY + roomHeight * 0.5f;
    CVector2 center(centerX, centerY);

    node->room = { roomX, roomY, roomWidth, roomHeight, center, Room::RoomType::eNormal};

    // 床の生成 (部屋の内側のみを床に設定)
    for (int y = roomY + 1; y < roomY + roomHeight - 1; y++)
    {
        for (int x = roomX + 1; x < roomX + roomWidth - 1; x++)
        {
            map[y][x].type = TileType::eFloor;
            mpRoomFloors.push_back(CVector2(static_cast<float>(x), static_cast<float>(y)));

            //// もし、部屋の角だった場合、
            //if (y == roomY + 1 && x == roomX + 1 ||
            //    y == roomY + 1 && x == roomX + roomWidth - 2 ||
            //    y == roomY + roomHeight - 2 && x == roomX + 1 ||
            //    y == roomY + roomHeight - 2 && x == roomX + roomWidth - 2)
            //{
            //    // ノード生成フラグをtrueにする
            //    map[y][x].node = true;
            //}
        }
    }
}

// 部屋の壁データの設定
void CBspMap::CreateRoomWall(const Room& room, std::vector<std::vector<Tile>>& map)
{
    // 上下の壁の設定
    for (int x = room.x; x < room.x + room.width; x++)
    {
        // 上の壁
        mMapData[room.y][x].type = TileType::eWall;
        mMapData[room.y][x].dir = Direction::eNorth;

        // 下の壁
        mMapData[room.y + room.height - 1][x].type = TileType::eWall;
        mMapData[room.y + room.height - 1][x].dir = Direction::eSouth;

    }
    // 左右の壁を設定
    for (int y = room.y; y < room.y + room.height; y++)
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

    // 繋がっていなければ
    if (!node->left->room.connected && !node->right->room.connected)
    {
        // 2点を直線で結ぶように通路を作る
        CreatePassage(map, node->left, node->right);

        node->left->room.connected = true;
        node->right->room.connected = true;
    }
}

// 部屋のランダムな座標を取得
CVector2 CBspMap::GetRoomRandomPos(SectionNode* node)
{
    if (!node) return CVector2();

    // 葉ノードを探す
    if (!node->left && !node->right)
    {
        // 床領域を2マス内側にした範囲
        int minX = node->room.x + 3;
        int maxX = node->room.x + node->room.width - 3;
        int minY = node->room.y + 3;
        int maxY = node->room.y + node->room.height - 3;

        // もし部屋の幅/高さが小さい場合、中央一点に固定
        if (minX > maxX) minX = maxX = (node->room.x + node->room.width / 2);
        if (minY > maxY) minY = maxY = (node->room.y + node->room.height / 2);

        int cx = Math::Rand(minX, maxX);  // ランダムに中央付近
        int cy = Math::Rand(minY, maxY);

        return CVector2(static_cast<float>(cx), static_cast<float>(cy));
    }

    // 子ノードを優先して探す
    if (node->left) return GetRoomRandomPos(node->left);
    if (node->right) return GetRoomRandomPos(node->right);

    return CVector2();
}

// 部屋同士の通路データの設定
void CBspMap::CreatePassage(std::vector<std::vector<Tile>>& map, SectionNode* nodeA, SectionNode* nodeB)
{
    // 左の子の部屋のランダムな座標を取得
    CVector2 leftRoom = GetRoomRandomPos(nodeA);
    // 右の子の部屋のランダムな座標を取得
    CVector2 rightRoom = GetRoomRandomPos(nodeB);

    // 座標を整数で取得
    float ax = leftRoom.X(), ay = leftRoom.Y();
    float bx = rightRoom.X(), by = rightRoom.Y();

    int startColX = -1, startColY = -1;

    // ラムダ式
    // [&]：使用する外部変数を全て参照渡しでキャプチャする
    // 通路データの書き換え
    auto carve = [&](int x, int y, Direction dir)
    {
        // 壁だった場合、
        if (map[y][x].type == TileType::eWall)
        {
            Direction curDir = map[y][x].dir;

            // 通路の進行方向と、壁の方向が一致(もしくは正反対)であれば、
            if (dir == curDir || dir == InverseDirection(curDir))
            {
                if (map[y][x + 1].type != TileType::eEntrance &&
                    map[y][x - 1].type != TileType::eEntrance &&
                    map[y + 1][x].type != TileType::eEntrance &&
                    map[y - 1][x].type != TileType::eEntrance)
                {
                    // マップのタイルを出入口に変更
                    map[y][x].type = TileType::eEntrance;
                }
            }
        }
        // 何も設定されていなければ、
        else if (map[y][x].type == TileType::None || map[y][x].type == TileType::eBoundary)
        {
            // 通路を設定
            map[y][x].type = TileType::ePassage;
            map[y][x].dir = dir;
        }

        // 部屋の床でなければ、
        if (map[y][x].type != TileType::eFloor)
        {
            // 通路フラグをオンにする
            map[y][x].passage = true;
            map[y][x].passageDir = dir;
        }

        //// 出入口であれば、
        //if (map[y][x].type == TileType::eEntrance)
        //{
        //    // 通路フラグをオフにする
        //    map[y][x].passage = false;
        //}
    };

    // 横方向
    int startX = static_cast<int>(std::min(ax, bx));
    int endX = static_cast<int>(std::max(ax, bx));
    // 縦方向
    int startY = static_cast<int>(std::min(ay, by));
    int endY = static_cast<int>(std::max(ay, by));

    bool pattern = Math::Rand(0, 1);

    // TODO:部屋をまたいで部屋が繋がった場合、まだ柱の隣が出入口になっている

    // 柱の隣の壁を出入口に変更しない処理
    // startXとendXの部屋の上下の端の床と同じ座標だった
    if (startX == nodeA->room.x + 1 || startX == nodeA->room.x + nodeA->room.width - 1 ||
        startX == nodeB->room.x + 1 || startX == nodeB->room.x + nodeB->room.width - 1 || 
        endY == nodeA->room.y + 1 || endY == nodeA->room.y + nodeA->room.height - 1 ||
        endY == nodeB->room.y + 1 || endY == nodeB->room.y + nodeB->room.height - 1)
    {
        // 縦→横で掘る
        pattern = false;
    }
    // startYとendYの部屋の左右の端の床と同じ座標だったら
    else if (startY == nodeA->room.y + 1 || startY == nodeA->room.y + nodeA->room.height - 1 ||
        startY == nodeB->room.y + 1 || startY == nodeB->room.y + nodeB->room.height - 1 ||
        endX == nodeA->room.x + 1 || endX == nodeA->room.x + nodeA->room.width - 1 ||
        endX == nodeB->room.x + 1 || endX == nodeB->room.x + nodeB->room.width - 1)
    {
        // 横→縦で掘る
        pattern = true;
    }

    // L字堀
    // 横→縦
    if (pattern)
    {
        // 横方向
        for (int x = startX; x <= endX; ++x)
        {
            carve(x, static_cast<int>(ay), Direction::eEast);
        }
        // 縦方向
        for (int y = startY; y <= endY; ++y)
        {
            carve(static_cast<int>(bx), y, Direction::eNorth);
        }
    }
    // 縦→横
    else
    {
        // 縦方向
        for (int y = startY; y <= endY; ++y)
        {
            carve(static_cast<int>(bx), y, Direction::eNorth);
        }
        // 横方向
        for (int x = startX; x <= endX; ++x)
        {
            carve(x, static_cast<int>(ay), Direction::eEast);
        }
    }

    //// Z字堀（縦→横→縦 or 横→縦→横）
    //bool verticalFirst = Math::Rand(0, 1);

    //// 中間点を決める（Z字の折れ点）
    //int midX = (ax + bx) / 2;
    //int midY = (ay + by) / 2;

    //if (verticalFirst)
    //{
    //    // 縦に掘る（a.y → midY）
    //    int sy = (ay < midY) ? ay : midY;
    //    int ey = (ay < midY) ? midY : ay;
    //    for (int y = sy; y <= ey; ++y)
    //        carve(ax, y, (ay < midY) ? Direction::eSouth : Direction::eNorth);

    //    // 横に掘る（ax → bx, y固定）
    //    int sx = std::min(ax, bx);
    //    int ex = std::max(ax, bx);
    //    for (int x = sx; x <= ex; ++x)
    //        carve(x, midY, (ax < bx) ? Direction::eEast : Direction::eWest);

    //    // 再度縦に掘る（midY → by）
    //    sy = (midY < by) ? midY : by;
    //    ey = (midY < by) ? by : midY;
    //    for (int y = sy; y <= ey; ++y)
    //        carve(bx, y, (midY < by) ? Direction::eSouth : Direction::eNorth);
    //}
    //else
    //{
    //    // 横→縦→横のZ字
    //    int sx = (ax < midX) ? ax : midX;
    //    int ex = (ax < midX) ? midX : ax;
    //    for (int x = sx; x <= ex; ++x)
    //        carve(x, ay, (ax < midX) ? Direction::eEast : Direction::eWest);

    //    int sy = std::min(ay, by);
    //    int ey = std::max(ay, by);
    //    for (int y = sy; y <= ey; ++y)
    //        carve(midX, y, (ay < by) ? Direction::eSouth : Direction::eNorth);

    //    sx = (midX < bx) ? midX : bx;
    //    ex = (midX < bx) ? bx : midX;
    //    for (int x = sx; x <= ex; ++x)
    //        carve(x, by, (midX < bx) ? Direction::eEast : Direction::eWest);
    //}

}

// 方角の正反対を返す
CBspMap::Direction CBspMap::InverseDirection(Direction dir) const
{
    switch (dir)
    {
    case Direction::eNorth: return Direction::eSouth;
    case Direction::eSouth: return Direction::eNorth;
    case Direction::eEast: return Direction::eWest;
    case Direction::eWest: return Direction::eEast;
    }

    return dir;
}

// 部屋の床の座標のリストからランダムに座標を取得
CVector CBspMap::GetRoomRandomFloorPos()
{
    // mpRoomFloorsが空の場合
    if (mpRoomFloors.empty())   return CVector::zero;

    int index = Math::Rand(0, mpRoomFloors.size() - 1);

    return CVector(mpRoomFloors[index].X() * TILE_SIZE, 0, 
                   mpRoomFloors[index].Y() * TILE_SIZE);
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
                // マップの範囲チェック
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
                // マップの範囲チェック
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
