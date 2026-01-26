#pragma once
#include "CCharaBase.h"
#include "CModel.h"

class CColliderMesh;

class CCrate : public CCharaBase
{
public:
	// コンストラクタ
	CCrate(const CVector& pos, ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	~CCrate();

	// 衝突判定
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	// 更新
	void Update() override;
	// 描画
	void Render() override;
private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

};