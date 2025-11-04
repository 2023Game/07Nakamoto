#pragma once
#include "CColliderTriangle.h"
#include "CBspMap.h"

class CBspMapCollider : public CObjectBase
{
public:
	// コンストラクタ
	CBspMapCollider();
	// デストラクタ
	~CBspMapCollider();

	// 床のコライダー
	void CreateFloorCollider(CBspMap::SectionNode* node);

	// 更新
	void Update() override;
private:
	std::vector<CColliderTriangle> mCollider;


};