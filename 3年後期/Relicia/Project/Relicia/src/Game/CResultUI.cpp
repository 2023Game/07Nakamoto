#include "CResultUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"
#include "CFont.h"
#include "CInventory.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// コンストラクタ
CResultUI::CResultUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mTotalSellPrice(0)
	, mpLogoFont(nullptr)
	, mpTotalSellPriceText(nullptr)
	, mpAppraisal(nullptr)
{
	// ゲームクリア画面の背景イメージを生成
	mpResultBg = new CImage
	(
		"UI\\Clear_bg.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	mpLogoFont = new CFont("res\\Font\\toroman.ttf");
	mpLogoFont->SetFontSize(128);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// 鑑定額の画像を生成
	mpAppraisal = new CImage
	(
		"UI\\appraisal.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpAppraisal->SetPos(CVector2(600.0f, 200.0f));

	// 合計価格のテキストを生成
	mpTotalSellPriceText = new CText
	(
		mpLogoFont, 128,
		CVector2(100.0f, 200.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	mTotalSellPrice = CInventory::Instance()->GetTotalSellPrice();

	mpTotalSellPriceText->SetText("%dG\n", mTotalSellPrice);
	mpTotalSellPriceText->SetEnableOutline(true);
	mpTotalSellPriceText->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// [コンティニュー]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// コンティニューの画像を読み込み
	btn1->LoadButtonImage("UI\\Continue0.png", "UI\\Continue1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CResultUI::OnClickStart, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [タイトルへ]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// タイトルへの画像を読み込み
	btn2->LoadButtonImage("UI\\title_he0.png", "UI\\title_he1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn2->SetOnClickFunc(std::bind(&CResultUI::OnClickTitle, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn2->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn2);
}

// デストラクタ
CResultUI::~CResultUI()
{
	SAFE_DELETE(mpResultBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// リザルト画面を終了するか
bool CResultUI::IsEnd() const
{
	return mIsEnd;
}

// ゲームをコンティニューするか
bool CResultUI::IsContinue() const
{
	// 選択項目が1つ目ならば、リトライ
	return mSelectIndex == 0;
}

// タイトルへ戻るか
bool CResultUI::IsTitle()
{
	// 選択項目が2つ目ならば、タイトルへ
	return mSelectIndex == 1;
}

// 更新
void CResultUI::Update()
{
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// フェードアウト
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpResultBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画
void CResultUI::Render()
{
	// 背景描画
	mpResultBg->Render();
	// 鑑定額の描画
	mpAppraisal->Render();
	// スコア描画
	mpTotalSellPriceText->Render();
	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// 待機状態
void CResultUI::UpdateIdle()
{
	switch (mStateStep)
	{
		// ステップ0：メニューの入場アニメーション
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// スケール値を一旦1.0より大きくして、1.0へ戻るイージングアニメーション
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
		// ステップ1：メニュー入場後の待ち
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// 一定時間待ったら、ボタンをオンにしてタッチできるようにする
			// （誤タッチを防ぐための待ち時間）
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// フェードアウト
void CResultUI::UpdateFadeOut()
{
}

// 状態切り替え
void CResultUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [コンティニュー]クリック時のコールバック関数
void CResultUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [タイトルへ]クリック時のコールバック関数
void CResultUI::OnClickTitle()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}