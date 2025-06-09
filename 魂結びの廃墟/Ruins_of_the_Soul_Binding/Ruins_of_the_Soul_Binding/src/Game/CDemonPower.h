#pragma once
#include "CInteractObject.h"

class CModel;
class CColliderMesh;

// 妖力の源のクラス
class CDemonPower : CInteractObject
{
public:
	// コンストラクタ
	CDemonPower(const CVector& pos);
	// デストラクタ
	~CDemonPower();

	// 調べる
	void Interact() override;

	// 破壊されたか
	bool BreakPower();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	//妖力も源のモデルデータ
	CCollider* mpCollider;	// 妖力の源のコリジョンデータ

	bool mBroken;	//	壊れたか
};