#include "CGaugeUI.h"
#include "CImage.h"
#include "CPlayer2.h"
#include "Maths.h"

CGaugeUI::CGaugeUI()
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpHpBar(nullptr)
	, mpRedBar(nullptr)
	, mBaseBarSize(0.0f, 0.0f)
	, mMaxPoint(100)
	, mCurPoint(mMaxPoint)
	, mPercent(1.0f)
{
	SetPos(WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5);

	// ダメージゲージを生成
	mpRedBar = new CImage
	(
		"UI\\hp_redbar.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mBaseBarSize = mpRedBar->GetSize();

	// HPゲージを生成
	mpHpBar = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	CVector2 center = CVector2(-mBaseBarSize.X() * 0.5f, 0.0f);
	mpHpBar->SetCenter(center);
}

CGaugeUI::~CGaugeUI()
{
	SAFE_DELETE(mpRedBar);
	SAFE_DELETE(mpHpBar);
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

// 更新
void CGaugeUI::Update()
{

}

// 描画
void CGaugeUI::Render()
{
	CVector2 pos = mPosition;

	// ゲージの描画
	mpRedBar->SetPos(pos);
	mpRedBar->Render();
	
	// バーの座標を設定
	CVector2 barPos = mPosition;
	barPos.X(barPos.X() - mBaseBarSize.X() * 0.5f);
	mpHpBar->SetPos(barPos);
	// ポイント残量の割合に合わせて、バーサイズを変更
	CVector2 barSize = mBaseBarSize;
	barSize.X(barSize.X() * mPercent);
	mpHpBar->SetSize(barSize);
	// バーの色を設定
	CColor barColor = CColor::green;
	if (mPercent <= 0.2f) barColor = CColor(1.0f, 0.5f, 0.0f, 1.0f);
	else if (mPercent <= 0.5f) barColor = CColor::yellow;
	mpHpBar->SetColor(barColor);
	// バーの描画
	mpHpBar->Render();
}

