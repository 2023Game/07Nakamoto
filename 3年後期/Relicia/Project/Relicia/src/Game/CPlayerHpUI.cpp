#include "CPlayerHpUI.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CPlayerHpUI::CPlayerHpUI(int point)
	: CUIBase(ETaskPriority::eUI, 0, ETaskPauseType::eGame)
	, mpWhiteImag(nullptr)
	, mpGaugeImag(nullptr)
	, mpFrameImage(nullptr)
	, mBaseBarSize(0.0f, 0.0f)
	, mMaxPoint(point)
	, mCurPoint(mMaxPoint)
	, mPercent(1.0f)
{
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

	// ゲージの枠を生成
	mpFrameImage = new CImage
	(
		"UI\\washi_gauge.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);

	//mpFireIcon = new CImage
	//(
	//	"UI\\fire_icon.png",
	//	ETaskPriority::eUI,
	//	0,
	//	ETaskPauseType::eGame,
	//	false,
	//	false
	//);
	//mpFireIcon->SetPos(CVector2(336.0f, 530.0f));
}

// デストラクタ
CPlayerHpUI::~CPlayerHpUI()
{
	SAFE_DELETE(mpGaugeImag);
	SAFE_DELETE(mpWhiteImag);
}

// 最大値を設定
void CPlayerHpUI::SetMaxPoint(int point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CPlayerHpUI::SetCurPoint(int point)
{
	mCurPoint = point;
	ApplyPoint();
}

// 現在のポイントをゲージに反映
void CPlayerHpUI::ApplyPoint()
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

// バーの色(白)
CColor CPlayerHpUI::CalcBarColor() const
{
	// バーの色を設定
	CColor barColor = CColor::green;
	if (mPercent <= 0.2f) barColor = CColor(1.0f, 0.5f, 0.0f, 1.0f);
	else if (mPercent <= 0.5f) barColor = CColor::yellow;
	return barColor;
}

// 更新
void CPlayerHpUI::Update()
{
}

// 描画
void CPlayerHpUI::Render()
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

	// 枠の描画
	mpFrameImage->SetPos(pos);
	mpFrameImage->Render();

	//mpFireIcon->Render();
}