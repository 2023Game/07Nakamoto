#pragma once
#include "CBillBoardImage.h"

class CHandGlow : public CBillBoardImage
{
public:
	// コンストラクタ
	CHandGlow(ETag tag);
	// デストラクタ
	~CHandGlow();

	/// <summary>
	/// 各パラメータを設定
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="dir">移動方向</param>
	void Setup(const CVector& pos, const CVector& dir);

	// ブレンドタイプを設定
	void SetBlendType(EBlend type);

	// 光のスケールの最大値を設定
	void SeGlowScale(float glowScale);
	// 光のスケール値が最大値になるまでの時間を設定
	void SetGlowScaleAnimTime(float glowScaleAnimTime);

	// 削除フラグが立っているかどうか
	bool IsDeath() const;

	// 更新
	void Update() override;

private:
	// アニメーションデータ
	static TexAnimData msAnimData;
	bool mIsDeath;		// 削除フラグ
	// 光のスケールの最大値
	float mGlowScale;
	// 光のスケール値が最大値になるまでの時間
	float mGlowScaleAnimTime;

};
