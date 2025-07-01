#pragma once
#include "CInteractObject.h"

class CModel;
class CCollider;
class CGaugeUI3D;

// 妖力の源のクラス
class CDemonPower : public CInteractObject
{
public:
	// コンストラクタ
	CDemonPower(const CVector& pos);
	// デストラクタ
	~CDemonPower();

	// 調べる
	void Interact() override;

	// 調べる内容のテキスト画像のパスを返す
	std::string GetInteractTextPath() const override;
	// 調べる内容のテキスト画像のパスを返す
	std::string GetInteractKeyPath() const override;

	// 破壊されたか
	bool BreakPower();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	//妖力も源のモデルデータ
	CCollider* mpCollider;	// 妖力の源のコリジョンデータ

	CGaugeUI3D* mpHpGauge;	// HPゲージ
	CVector mGaugeOffsetPos;	// ゲージのオフセット位置

	bool mBroken;	//	壊れたか
};