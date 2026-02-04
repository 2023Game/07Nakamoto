#pragma once
#include "CCharaBase.h"
#include "CModel.h"

class CColliderBox;

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
	CColliderBox* mpColliderBox;

	bool mPush;
	bool mLastPush;

	float mElapsedTime;
};