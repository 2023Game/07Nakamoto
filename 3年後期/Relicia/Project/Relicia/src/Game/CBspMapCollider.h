#pragma once
#include "CColliderTriangle.h"

class CBspMap;

class CBspMapCollider
{
public:
	// コンストラクタ
	CBspMapCollider(CBspMap::SectionNode* node);
	// デストラクタ
	~CBspMapCollider();

};