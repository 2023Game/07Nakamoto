#include "CGaugeUI.h"
#include "CImage.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CPlayerManager.h"

CGaugeUI::CGaugeUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpWhiteImag(nullptr)
	, mpGaugeImag(nullptr)
	, mpIcon(nullptr)
	, mpIconFrame(nullptr)
	, mBaseBarSize(0.0f, 0.0f)
	, mMaxPoint(100)
	, mCurPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// 中央の座標
	//SetPos(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5);

	// ダメージゲージを生成
	mpGaugeImag = new CImage
	(
		"UI\\hp_redbar.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mBaseBarSize = mpGaugeImag->GetSize();

	// HPゲージを生成
	mpWhiteImag = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	CVector2 center = CVector2(-mBaseBarSize.X() * 0.5f, 0.0f);
	mpWhiteImag->SetCenter(center);

	// アイコンを生成
	mpIcon = new CImage
	(
		"UI\\Silhouette\\cat_girl.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIcon->SetPos(CVector::zero);

	// アイコンのフレーム
	mpIconFrame = new CImage
	(
		"UI\\Silhouette\\frame.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIconFrame->SetPos(CVector::zero);
}

CGaugeUI::~CGaugeUI()
{
	SAFE_DELETE(mpGaugeImag);
	SAFE_DELETE(mpWhiteImag);
	SAFE_DELETE(mpIcon);
	SAFE_DELETE(mpIconFrame);

}

// 最大値を設定
void CGaugeUI::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CGaugeUI::SetCurPoint(int point)
{
	mCurPoint = point;
	ApplyPoint();
}

// 現在のポイントをゲージに反映
void CGaugeUI::ApplyPoint()
{
	// 最大値が不正値でなければ、
	if (mMaxPoint > 0)
	{
		// 残りの値から割合を求める
		mPercent = Math::Clamp01((float)mCurPoint / mMaxPoint);
	}
	// 不正値だった場合は、1.0で固定
	else
	{
		mPercent = 1.0f;
	}
}

CColor CGaugeUI::CalcBarColor() const
{
	return CColor::white;
}

// 更新
void CGaugeUI::Update()
{
	CPlayerManager* pmg = CPlayerManager::Instance();
	//pmg->
}

// 描画
void CGaugeUI::Render()
{
	CVector2 pos = mPosition;

	// ゲージの描画
	mpGaugeImag->SetPos(pos);
	mpGaugeImag->Render();
	
	// バーの座標を設定
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - mBaseBarSize.X() * 0.5f);
	mpWhiteImag->SetPos(barPos);
	// ポイント残量の割合に合わせて、バーサイズを変更
	CVector2 barSize = mBaseBarSize;
	barSize.X(barSize.X() * mPercent);
	mpWhiteImag->SetSize(barSize);
	// バーの色を設定
	CColor barColor = CalcBarColor();
	mpWhiteImag->SetColor(barColor);
	// バーの描画
	mpWhiteImag->Render();

	// アイコンの描画
	mpIcon->Render();
	// アイコンのフレーム描画
	mpIconFrame->Render();
}

