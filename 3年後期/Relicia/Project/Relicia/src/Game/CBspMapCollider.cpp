#include "CBspMapCollider.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider(CBspMap::SectionNode* node)
    : CObjectBase(ETag::eField, 
        ETaskPriority::eBackground, 0,
        ETaskPauseType::eGame)
{
    CreateFloorCollider(node);
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

void CBspMapCollider::CreateFloorCollider(CBspMap::SectionNode* node)
{
    if (!node) return;

    // 再帰
    CreateFloorCollider(node->left);
    CreateFloorCollider(node->right);

    // 葉ノードかどうか(部屋があるか)
    //if (node->room.width > 0 && node->room.height > 0)
    if(!node->left || !node->right)
    {
        // 床の三角形コライダー
        CColliderTriangle* collider = new CColliderTriangle(this, ELayer::eField,
            CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + node->room.height - 1) * TILE_SIZE),
            CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + 1) * TILE_SIZE),
            CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + 1) * TILE_SIZE)
        );

        mpCollider.push_back(collider);

        // 床の三角形コライダー
        collider = new CColliderTriangle(this, ELayer::eField,
            CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + 1) * TILE_SIZE),
            CVector((node->room.x + 1) * TILE_SIZE,                    3, (node->room.y + node->room.height - 1) * TILE_SIZE),
            CVector((node->room.x + node->room.width - 1) * TILE_SIZE, 3, (node->room.y + node->room.height - 1) * TILE_SIZE));

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

