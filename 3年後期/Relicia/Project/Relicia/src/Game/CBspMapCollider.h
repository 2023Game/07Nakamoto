#pragma once
#include "CColliderTriangle.h"
#include "CBspMap.h"

class CBspMapCollider : public CObjectBase
{
public:
	// コンストラクタ
	CBspMapCollider(CBspMap* map);
	// デストラクタ
	~CBspMapCollider();

	// 更新
	void Update() override;
private:
	// 部屋のコライダーを生成
	void CreateRoomCollider(const CBspMap::SectionNode* node);
	// 床のコライダー生成
	void CreateFloorCollider(const CBspMap::SectionNode* node);
	// 壁のコライダー生成
	void CreateWallCollider(CBspMap* map);

	std::vector<CColliderTriangle*> mpCollider;


};