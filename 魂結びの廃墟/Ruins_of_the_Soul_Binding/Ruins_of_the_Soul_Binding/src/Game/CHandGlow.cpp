#include "CHandGlow.h"
#include "Easing.h"

// 光のスケール値の最大値
#define GLOW_SCALE 10.0f
// 光のスケール値が最大値になるまでの時間
#define GLOW_SCALE_ANIM_TIME 3.0f
// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0625f
// 光のエフェクトのアニメーションデータ
TexAnimData CHandGlow::msAnimData = TexAnimData(1, 10, true, 64, ANIM_TIME);

// コンストラクタ
CHandGlow::CHandGlow(ETag tag)
	:CBillBoardImage("Effect\\handglow.png",tag,ETaskPauseType::eGame)
	, mIsDeath(false)
	, mGlowScale(GLOW_SCALE)
	, mGlowScaleAnimTime(GLOW_SCALE_ANIM_TIME)
{
	SetAnimData(&msAnimData);
	SetAlpha(0.7f);
}

// デストラクタ
CHandGlow::~CHandGlow()
{
}

// 各パラメータを設定
void CHandGlow::Setup(const CVector& pos, const CVector& dir)
{
	Position(pos);
}

// ブレンドタイプを設定
void CHandGlow::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 光のスケールの最大値を設定
void CHandGlow::SeGlowScale(float glowScale)
{
	mGlowScale = glowScale;
}

// 光のスケール値が最大値になるまでの時間を設定
void CHandGlow::SetGlowScaleAnimTime(float glowScaleAnimTime)
{
	mGlowScaleAnimTime = glowScaleAnimTime;
}

// 削除フラグが立っているかどうか
bool CHandGlow::IsDeath() const
{
	return mIsDeath;
}

// 更新
void CHandGlow::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();


}
