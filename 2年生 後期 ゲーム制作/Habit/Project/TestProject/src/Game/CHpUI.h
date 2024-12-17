#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CHpUI : public CUIBase
{
public:
	// コントラクタ
	CHpUI();
	// デストラクタ
	~CHpUI();

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

	CImage* mpBlueBar;
	CImage* mpRedBar;
	CVector2 mBaseBarSize;

	int mMaxPoint;	// 最大値
	int mCurPoint;	// 現在値
	float mPercent;	// ポイント残量の割合

};