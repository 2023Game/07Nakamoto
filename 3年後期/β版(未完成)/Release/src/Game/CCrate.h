#pragma once
#include "CObjectBase.h"
#include "CModel.h"

class CColliderMesh;

class CCrate : public CObjectBase
{
public:
	// コンストラクタ
	CCrate(const CVector& pos);
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