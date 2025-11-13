#pragma once
#include "CUIBase.h"

class CImage;

class CPlayerUI :public CUIBase
{
public:
	// コンストラクタ
	CPlayerUI(int point);
	// デストラクタ
	~CPlayerUI();

	// 最大値を設定
	void SetMaxPoint(int point);
	// 現在値を設定
	void SetCurPoint(int point);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 現在のポイントをゲージに反映
	void ApplyPoint();

	// バーの色(白)
	virtual CColor CalcBarColor() const;

	CImage* mpWhiteImag;	// ゲージのイメージ
	CImage* mpGaugeImag;	// 白イメージ
	CImage* mpFrameImage;	// ゲージの枠のイメージ

	CVector2 mGaugeSize;

	CVector2 mBaseBarSize;	// ゲージのイメージサイズ
	int mMaxPoint;	// 最大値
	int mCurPoint;	// 現在値
	float mPercent;	// ポイント残量の割合
};
