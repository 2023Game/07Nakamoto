#include "CBspMapCollider.h"
#include "CBspMap.h"
#include "CColliderTriangle.h"

// コンストラクタ
CBspMapCollider::CBspMapCollider(const CBspMap::SectionNode* node)
{
    if (!node || node->right || node->left) return;


}

// デストラクタ
CBspMapCollider::~CBspMapCollider()
{
}
