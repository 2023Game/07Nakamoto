#include "CBspMapCollider.h"
#include "CField.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider(CBspMap* map)
    : CObjectBase(ETag::eField, ETaskPriority::eBackground, 0,ETaskPauseType::eGame)
{
    if (!map) return;

    // 部屋の床コライダー生成
    CreateRoomCollider(map->GetRootNode());
    // 部屋の壁コライダー生成
    CreateWallCollider(map);
    // 通路の床コライダー生成
    CreatePassage(map);
}

// デストラクタ
CBspMapCollider::~CBspMapCollider()
{
    // 生成したコライダーを全て削除
    for (CColliderTriangle* col : mpCollider)
    {
        delete col;
    }
    mpCollider.clear();
}

// 通路のコライダーを生成
void CBspMapCollider::CreatePassage(CBspMap* map)
{
    for (CBspMap::TileSegment seg : map->GetSegments())
    {
        switch (seg.type)
        {
        // 通路の場合
        case CBspMap::TileType::ePassage:
            // 東(横)方向だったら
            if (seg.dir == CBspMap::Direction::eEast)
            {
                // 床の三角形コライダー
                CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eFloor,
                    CVector(seg.end.X() * TILE_SIZE, 0, seg.end.Y() * TILE_SIZE + TILE_SIZE),
                    CVector(seg.end.X() * TILE_SIZE, 0, seg.end.Y() * TILE_SIZE),
                    CVector(seg.start.X() * TILE_SIZE, 0, seg.start.Y() * TILE_SIZE)
                );
                mpCollider.push_back(collider);

                // 床の三角形コライダー
                collider = new CColliderTriangle(this, ELayer::eFloor,
                    CVector(seg.start.X() * TILE_SIZE, 0, seg.start.Y() * TILE_SIZE),
                    CVector(seg.start.X() * TILE_SIZE, 0, seg.start.Y() * TILE_SIZE + TILE_SIZE),
                    CVector(seg.end.X() * TILE_SIZE, 0, seg.end.Y() * TILE_SIZE + TILE_SIZE)
                );
                mpCollider.push_back(collider);
            
            }
            // 北(縦)方向だったら
            else if (seg.dir == CBspMap::Direction::eNorth)
            {
                // 床の三角形コライダー
                CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eFloor,
                    CVector(seg.end.X() * TILE_SIZE, 0, seg.end.Y() * TILE_SIZE),
                    CVector(seg.end.X() * TILE_SIZE + TILE_SIZE, 0, seg.end.Y() * TILE_SIZE),
                    CVector(seg.start.X() * TILE_SIZE, 0, seg.start.Y() * TILE_SIZE)
                );

                mpCollider.push_back(collider);

                // 床の三角形コライダー
                collider = new CColliderTriangle(this, ELayer::eFloor,
                    CVector(seg.start.X() * TILE_SIZE + TILE_SIZE, 0, seg.start.Y() * TILE_SIZE),
                    CVector(seg.start.X() * TILE_SIZE, 0, seg.start.Y() * TILE_SIZE),
                    CVector(seg.end.X() * TILE_SIZE + TILE_SIZE, 0, seg.end.Y() * TILE_SIZE)
                );

                mpCollider.push_back(collider);
            }

            break;
        // 壁の場合
        case CBspMap::TileType::eWall:

            if (seg.dir == CBspMap::Direction::eSouth ||
                seg.dir == CBspMap::Direction::eWest)
            {
                std::swap(seg.start, seg.end);
            }

            int offsetX = 0;
            int offsetY = 0;

            switch (seg.dir)
            {
            case CBspMap::Direction::eNorth:    offsetY = -2;    break;
            case CBspMap::Direction::eEast:     offsetX = -TILE_SIZE + 2;    break;
            case CBspMap::Direction::eSouth:    offsetY = -TILE_SIZE + 2;    break;
            case CBspMap::Direction::eWest:     offsetX = -2;    break;
            }

            // 壁の三角形コライダー
            CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eWall,
                CVector(seg.end.X() * TILE_SIZE - offsetX, 0, seg.end.Y() * TILE_SIZE - offsetY),
                CVector(seg.end.X() * TILE_SIZE - offsetX, 40, seg.end.Y() * TILE_SIZE - offsetY),
                CVector(seg.start.X() * TILE_SIZE - offsetX, 40, seg.start.Y() * TILE_SIZE - offsetY));

            mpCollider.push_back(collider);

            // 壁の三角形コライダー
            collider = new CColliderTriangle(this, ELayer::eWall,
                CVector(seg.end.X() * TILE_SIZE - offsetX, 0, seg.end.Y() * TILE_SIZE - offsetY),
                CVector(seg.start.X() * TILE_SIZE - offsetX, 40, seg.start.Y() * TILE_SIZE - offsetY),
                CVector(seg.start.X() * TILE_SIZE - offsetX, 0, seg.start.Y() * TILE_SIZE - offsetY));

            mpCollider.push_back(collider);

            break;
        }

    }
}

// 部屋のコライダーを生成
void CBspMapCollider::CreateRoomCollider(const CBspMap::SectionNode* node)
{
    if (!node) return;

    // 葉ノードかどうか(部屋のノードかどうか)
    if (!node->left && !node->right)
    {
        // 床のコライダー生成
        CreateFloorCollider(node);

        return;
    }

    // 再帰
    CreateRoomCollider(node->left);
    CreateRoomCollider(node->right);
}

// 部屋の床のコライダー生成
void CBspMapCollider::CreateFloorCollider(const CBspMap::SectionNode* node)
{
    // 床の三角形コライダー
    CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eFloor,
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + node->room.height - 1) * TILE_SIZE),
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + 1) * TILE_SIZE),
        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + 1) * TILE_SIZE)
    );

    mpCollider.push_back(collider);

    // 床の三角形コライダー
    collider = new CColliderTriangle(this, ELayer::eFloor,
        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + 1) * TILE_SIZE),
        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + node->room.height - 1) * TILE_SIZE),
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + node->room.height - 1) * TILE_SIZE));

    mpCollider.push_back(collider);
}

// 壁のコライダー生成
void CBspMapCollider::CreateWallCollider(CBspMap* map)
{
    for (CBspMap::TileSegment seg : map->CollectWallSegments())
    {
        if (seg.type != CBspMap::TileType::eWall) continue;

        if (seg.dir == CBspMap::Direction::eSouth ||
            seg.dir == CBspMap::Direction::eWest)
        {
            std::swap(seg.start, seg.end);
        }

        int offsetX = 0;
        int offsetY = 0;

        switch (seg.dir)
        {
        case CBspMap::Direction::eNorth:    offsetY = -TILE_SIZE - 2;    break;
        case CBspMap::Direction::eEast:     offsetX = 2;    break;
        case CBspMap::Direction::eSouth:    offsetY = 2;    break;
        case CBspMap::Direction::eWest:     offsetX = -TILE_SIZE - 2;    break;
        }

        // 壁の三角形コライダー
        CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eWall,
            CVector(seg.end.X() * TILE_SIZE - offsetX, 0, seg.end.Y() * TILE_SIZE - offsetY),
            CVector(seg.end.X() * TILE_SIZE - offsetX, 40, seg.end.Y() * TILE_SIZE - offsetY),
            CVector(seg.start.X() * TILE_SIZE - offsetX, 40, seg.start.Y() * TILE_SIZE - offsetY));

        mpCollider.push_back(collider);

        // 壁の三角形コライダー
        collider = new CColliderTriangle(this, ELayer::eWall,
            CVector(seg.end.X() * TILE_SIZE - offsetX, 0, seg.end.Y() * TILE_SIZE - offsetY),
            CVector(seg.start.X() * TILE_SIZE - offsetX, 40, seg.start.Y() * TILE_SIZE - offsetY),
            CVector(seg.start.X() * TILE_SIZE - offsetX, 0, seg.start.Y() * TILE_SIZE - offsetY));

        mpCollider.push_back(collider);
    }
}

// 更新
void CBspMapCollider::Update()
{
    for (CColliderTriangle* col : mpCollider)
    {
        col->Update();
    }
}

