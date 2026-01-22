#include "CBspMapCollider.h"
#include "CField.h"
#include "CNavManager.h"
#include "CColliderMesh.h"

#define WALL_COL_OFFSET 2.0f

// コンストラクタ
CBspMapCollider::CBspMapCollider(CBspMap* map, int x, int y)
    : CObjectBase(ETag::eField, ETaskPriority::eBackground, 0,ETaskPauseType::eGame)
    , mpFloorCollider(nullptr)
    , mpWallCollider(nullptr)
{
    if (!map) return;
    
    // 区画全体の床コライダーを生成
    CreateOptimaizedFloorMeshCollider(x, y);

    mpWallCollider = new CColliderMesh
    (
        this, ELayer::eWall, nullptr,
        true, 1.0f,
        10, 1, 10
    );

    // 壁コライダーの生成
    for (CBspMap::TileSegment seg : map->GetWallSegments())
    {
        CreateWallCollider(&seg);
    }

    // 壁コライダーの分割情報を更新
    mpWallCollider->DivisionMesh();

    // 経路探索用の遮蔽物チェックのコライダーに、壁のコライダーを登録
    CNavManager::Instance()->AddCollider(mpWallCollider);

    //mpFloorCollider->SetShow(true);
    //mpWallCollider->SetShow(true);
}

// デストラクタ
CBspMapCollider::~CBspMapCollider()
{
    // 生成したコライダーを全て削除
    SAFE_DELETE(mpFloorCollider);
    SAFE_DELETE(mpWallCollider);
}

//// 部屋のコライダーを生成
//void CBspMapCollider::CreateRoomCollider(const CBspMap::SectionNode* node)
//{
//    if (!node) return;
//
//    // 葉ノードかどうか(部屋のノードかどうか)
//    if (!node->left && !node->right)
//    {
//        // 床のコライダー生成
//        CreateFloorCollider(node);
//
//        return;
//    }
//
//    // 再帰
//    CreateRoomCollider(node->left);
//    CreateRoomCollider(node->right);
//}

//// 部屋の床のコライダー生成
//void CBspMapCollider::CreateFloorCollider(const CBspMap::SectionNode* node)
//{
//    // 床の三角形コライダー
//    CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eFloor,
//        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + node->room.height - 1) * TILE_SIZE),
//        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + 1) * TILE_SIZE),
//        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + 1) * TILE_SIZE)
//    );
//
//    mpCollider.push_back(collider);
//
//    // 床の三角形コライダー
//    collider = new CColliderTriangle(this, ELayer::eFloor,
//        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + 1) * TILE_SIZE),
//        CVector((node->room.x + 1) * TILE_SIZE,                    0, (node->room.y + node->room.height - 1) * TILE_SIZE),
//        CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 0, (node->room.y + node->room.height - 1) * TILE_SIZE));
//
//    mpCollider.push_back(collider);
//}

// 壁のコライダー生成
void CBspMapCollider::CreateWallCollider(CBspMap::TileSegment* seg)
{
    if (seg->dir == CBspMap::Direction::eNorth ||
        seg->dir == CBspMap::Direction::eEast)
    {
        std::swap(seg->start, seg->end);
    }

    float offsetX = 0;
    float offsetY = 0;

    // 方角によってオフセットを設定
    switch (seg->dir)
    {
        // 北
    case CBspMap::Direction::eNorth:
        offsetX = TILE_SIZE * 0.5f;
        offsetY = TILE_SIZE * 0.5f - WALL_COL_OFFSET;
        break;
        // 東
    case CBspMap::Direction::eEast:
        offsetX = -TILE_SIZE * 0.5f + WALL_COL_OFFSET;
        offsetY = TILE_SIZE * 0.5f;
        break;
        // 南
    case CBspMap::Direction::eSouth:
        offsetX = TILE_SIZE * 0.5f;
        offsetY = -TILE_SIZE * 0.5f + WALL_COL_OFFSET;
        break;
        // 西
    case CBspMap::Direction::eWest:
        offsetX = TILE_SIZE * 0.5f - WALL_COL_OFFSET;
        offsetY = TILE_SIZE * 0.5f;
        break;
    }

    // 壁の三角形コライダー
    mpWallCollider->AddTriangle
    (
        CVector(seg->start.X() * TILE_SIZE - offsetX, 40, seg->start.Y() * TILE_SIZE - offsetY),
        CVector(seg->end.X() * TILE_SIZE - offsetX, 40, seg->end.Y() * TILE_SIZE - offsetY),
        CVector(seg->end.X() * TILE_SIZE - offsetX, 0, seg->end.Y() * TILE_SIZE - offsetY)
    );

    // 壁の三角形コライダー
    mpWallCollider->AddTriangle
    (
        CVector(seg->start.X() * TILE_SIZE - offsetX, 40, seg->start.Y() * TILE_SIZE - offsetY),
        CVector(seg->end.X() * TILE_SIZE - offsetX, 0, seg->end.Y() * TILE_SIZE - offsetY),
        CVector(seg->start.X() * TILE_SIZE - offsetX, 0, seg->start.Y() * TILE_SIZE - offsetY)
    );


}

// 床全体の四角形コライダーを生成
void CBspMapCollider::CreateOptimaizedFloorMeshCollider(int x, int y)
{
    // 床のコライダーを生成
    mpFloorCollider = new CColliderMesh
    (
        this, ELayer::eFloor, nullptr,
        true, 1.0f,
        1, 1, 1
    );

    // マップ全域を覆う三角形ポリゴンを2つ追加
    mpFloorCollider->AddTriangle
    (
        CVector(x * TILE_SIZE, 0, y * TILE_SIZE),
        CVector(x * TILE_SIZE, 0, 0),
        CVector(0, 0, 0)
    );

    // 床の三角形コライダー
    mpFloorCollider->AddTriangle
    (
        CVector(x * TILE_SIZE, 0, y * TILE_SIZE),
        CVector(0, 0, 0),
        CVector(0, 0, y * TILE_SIZE)
    );

    // 登録されている三角形ポリゴンの分割状態を更新
    mpFloorCollider->DivisionMesh();
}

// 床コライダーを取得
CColliderMesh* CBspMapCollider::GetFloorCollider() const
{
    return mpFloorCollider;
}

// 壁コライダーを取得
CColliderMesh* CBspMapCollider::GetWallCollider() const
{
    return mpWallCollider;
}

// 更新
void CBspMapCollider::Update()
{
    mpFloorCollider->Update();
    mpWallCollider->Update();
}

