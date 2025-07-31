#include "CGameClearUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// メニューのアニメーション時間
#define OPEN_ANIM_TIME 0.25f
// メニューのアニメーション後の待ち時間
#define OPENED_WAIT_TIME 0.5f

// コンストラクタ
CGameClearUI::CGameClearUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// ゲームクリア画面の背景イメージを生成
	mpGameClearBg = new CImage
	(
		"UI\\clear.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [タイトルへ]ボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// タイトルへの画像を読み込み
	btn1->LoadButtonImage("UI\\title_he0.png", "UI\\title_he1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// [おわる]ボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(180.0f, 52.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// タイトルへの画像を読み込み
	btn2->LoadButtonImage("UI\\Title\\title_quit0.png", "UI\\Title\\title_quit1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn2->SetOnClickFunc(std::bind(&CGameClearUI::OnClickQuit, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn2->SetEnable(false);
	// ボタンリストに追加
	mButtons.push_back(btn2);

}

// デストラクタ
CGameClearUI::~CGameClearUI()
{
	SAFE_DELETE(mpGameClearBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// ゲームクリア画面終了か
bool CGameClearUI::IsEnd() const
{
	return mIsEnd;
}

// タイトル画面にするか
bool CGameClearUI::IsTitle() const
{
	// 選択項目が1つ目ならば、タイトル画面
	return mSelectIndex == 0;
}

// ゲームを終了するか
bool CGameClearUI::IsExitGame() const
{
	// 選択項目が2つ目ならば、ゲーム終了
	return mSelectIndex == 1;
}

// 更新処理
void CGameClearUI::Update()
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

	mpGameClearBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画処理
void CGameClearUI::Render()
{
	// 背景描画
	mpGameClearBg->Render();
	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// 待機状態
void CGameClearUI::UpdateIdle()
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
void CGameClearUI::UpdateFadeOut()
{
}

// 状態切り替え
void CGameClearUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [タイトルへ]クリック時のコールバック関数
void CGameClearUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [おわる]クリック時のコールバック関数
void CGameClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
