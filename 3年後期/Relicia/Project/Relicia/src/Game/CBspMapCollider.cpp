#include "CBspMapCollider.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider()
{

}

// デストラクタ
CBspMapCollider::~CBspMapCollider()
{
}

void CBspMapCollider::CreateFloorCollider(CBspMap::SectionNode* node)
{
    if (!node || !node->right || !node->left) return;

    // 再帰
    CreateFloorCollider(node->left);
    CreateFloorCollider(node->right);

    if (!node->right || node->left)
    {
        // 床の三角形コライダー
        CColliderTriangle collider = CColliderTriangle(this, ELayer::eField,
            CVector(node->room.x + 1, 1, node->room.y + 1),
            CVector((node->room.x + node->room.width) * TILE_SIZE, 1, node->room.y - 1),
            CVector((node->room.x + node->room.width) * TILE_SIZE, 1, (node->room.y + node->room.height - 1) * TILE_SIZE));

        mCollider.push_back(collider);

        // 床の三角形コライダー
        collider = CColliderTriangle(this, ELayer::eField,
            CVector(node->room.x + 1, 1, node->room.y + 1),
            CVector(node->room.x + 1, 1, (node->room.y + node->room.height - 1) * TILE_SIZE),
            CVector((node->room.x + node->room.width) * TILE_SIZE, 1, (node->room.y + node->room.height - 1) * TILE_SIZE));

        mCollider.push_back(collider);
    }
}

// 更新
void CBspMapCollider::Update()
{
}

