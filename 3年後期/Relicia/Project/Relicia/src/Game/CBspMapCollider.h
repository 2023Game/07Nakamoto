#pragma once
#include "CColliderTriangle.h"
#include "CBspMap.h"

//class CBspMap;

class CBspMapCollider
{
public:
	// コンストラクタ
	CBspMapCollider(CBspMap::SectionNode* node);
	// デストラクタ
	~CBspMapCollider();
private:
	std::vector<CColliderTriangle> mCollider;
};