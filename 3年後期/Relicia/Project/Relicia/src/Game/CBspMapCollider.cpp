#include "CBspMapCollider.h"
#include "CField.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider(CBspMap* map)
    : CObjectBase(ETag::eField, 
        ETaskPriority::eBackground, 0,
        ETaskPauseType::eGame)
{
    if (!map) return;

    CreateRoomCollider(map->GetRootNode());

    CreateWallCollider(map);
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

// 床のコライダー生成
void CBspMapCollider::CreateFloorCollider(const CBspMap::SectionNode* node)
{
    // 床の三角形コライダー
    CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eFloor,
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + node->room.height - 1) * TILE_SIZE),
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + 1) * TILE_SIZE),
        CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + 1) * TILE_SIZE)
    );

    mpCollider.push_back(collider);

    // 床の三角形コライダー
    collider = new CColliderTriangle(this, ELayer::eFloor,
        CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + 1) * TILE_SIZE),
        CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + node->room.height - 1) * TILE_SIZE),
        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + node->room.height - 1) * TILE_SIZE));

    mpCollider.push_back(collider);
}

// 壁のコライダー生成
void CBspMapCollider::CreateWallCollider(CBspMap* map)
{
    for (CBspMap::WallSegment seg : map->CollectWallSegments())
    {
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
            
        default:
            break;
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

