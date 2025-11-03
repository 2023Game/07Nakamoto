#include "CBspMapCollider.h"
#include "CBspMap.h"
#include "CColliderTriangle.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider(CBspMap::SectionNode* node)
{
    if (!node || node->right || node->left) return;

    CColliderTriangle collider = CColliderTriangle(&node->room, ELayer::eField,
        CVector(node->room.x + 1, 0, node->room.y + 1),
        CVector((node->room.x + node->room.width) * 20.0f, 0, node->room.y - 1),
        CVector((node->room.x + node->room.width) * 20.0f, 0, (node->room.y + node->room.height - 1) * 20.0f));

    mCollider.push_back(collider);

   collider = CColliderTriangle(&node->room, ELayer::eField,
        CVector(node->room.x + 1, 0, node->room.y + 1),
        CVector(node->room.x + 1, 0, (node->room.y + node->room.height - 1) * 20.0f),
        CVector((node->room.x + node->room.width) * 20.0f, 0, (node->room.y + node->room.height - 1) * 20.0f));

    mCollider.push_back(collider);

}

// デストラクタ
CBspMapCollider::~CBspMapCollider()
{
}
