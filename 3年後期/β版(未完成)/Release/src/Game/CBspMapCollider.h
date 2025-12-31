#pragma once
#include "CColliderTriangle.h"
#include "CBspMap.h"

class CBspMapCollider : public CObjectBase
{
public:
	// コンストラクタ
	CBspMapCollider(CBspMap* map, int x, int y);
	// デストラクタ
	~CBspMapCollider();

	// 床全体の四角形コライダーを生成
	void CreateOptimaizedFloorMeshCollider(int x, int y);

	// 床コライダーを取得
	CColliderMesh* GetFloorCollider() const;
	// 壁コライダーを取得
	CColliderMesh* GetWallCollider() const;

	// 更新
	void Update() override;
private:
	// 部屋のコライダーを生成
	//void CreateRoomCollider(const CBspMap::SectionNode* node);
	// 部屋の床のコライダー生成
	//void CreateFloorCollider(const CBspMap::SectionNode* node);
	// 壁のコライダー生成
	void CreateWallCollider(CBspMap::TileSegment* seg);

	CColliderMesh* mpFloorCollider;	// マップの床のコライダー
	CColliderMesh* mpWallCollider;	// マップの壁のコライダー
};