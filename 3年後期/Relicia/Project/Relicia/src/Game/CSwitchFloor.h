#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

class CSwitchFloor : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchFloor(const CVector& pos);
	// デストラクタ
	~CSwitchFloor();

	// ボタンのオンオフを切り替える
	void ChangeSwith();
	// 衝突判定
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
private:
	CModel* mpModel;
	CCollider* mpCollider;

	CObjectBase* mpLastPushedObject;	// 前のフレームで乗っていたオブジェクト
	CObjectBase* mpPushedObject;	// 乗っているオブジェクト

	bool mSwitch;
};