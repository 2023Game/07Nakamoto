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

	// 通路のコライダーを生成
	void CreatePassage(CBspMap* map);
	// 床全体の四角形コライダーを生成
	void CreateOptimaizedFloorMeshCollider(int x, int y);

	// コライダーを取得
	const std::vector<CColliderTriangle*> GetCollider() const;

	// 更新
	void Update() override;
private:
	// 部屋のコライダーを生成
	void CreateRoomCollider(const CBspMap::SectionNode* node);
	// 部屋の床のコライダー生成
	void CreateFloorCollider(const CBspMap::SectionNode* node);
	// 壁のコライダー生成
	void CreateWallCollider(CBspMap* map);


	std::vector<CColliderTriangle*> mpCollider;


};