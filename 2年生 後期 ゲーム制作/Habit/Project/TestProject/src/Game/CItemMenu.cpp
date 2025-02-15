#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"
#include "CInventory.h"
#include "CExpandButton.h"

#define BACK_SIZE 60.0f
#define BACK_ADJUST_POS CVector2(30.0f,0.0f)	//背景の調整位置

CItemMenu* CItemMenu::spInstance = nullptr;

// インスタンスを取得
CItemMenu* CItemMenu::Instance()
{
	return spInstance;
}

// コンストラクト
CItemMenu::CItemMenu()
	: CUIBase(ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false)
	, mState(EState::eIdle)
	, mpBack(nullptr)
	, mpText(nullptr)
	, mIsOpened(false)
{
	spInstance = this;

	// 文字描画の背景
	mpBack = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetSize(BACK_SIZE, BACK_SIZE);
	mpBack->SetAlpha(0.8f);

	// 使うボタンを生成
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(0.0f, 0.0f),
		CVector2(30.0f, 30.0f), 
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI\\title_quit0.png", "UI\\title_quit1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	//btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn1->SetEnable(false);
	//btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// 戻るボタンを生成
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(0.0f, 0.0f),
		CVector2(30.0f, 30.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn2->LoadButtonImage("UI\\title_quit0.png", "UI\\title_quit1.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	//btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// ボタンは最初は無効化して、スケール値を0にしておく
	btn2->SetEnable(false);
	//btn1->SetScale(0.0f);
	// ボタンリストに追加
	mButtons.push_back(btn2);
	SetEnable(false);
}

// デストラクタ
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	SAFE_DELETE(mpBack);
	SAFE_DELETE(mpText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();

}

// 更新
void CItemMenu::Update()
{

	mpBack->SetPos(mPosition + BACK_ADJUST_POS);
	mpBack->Update();

	if (!CInventory::Instance()->IsEnable())
	{
		SetEnable(false);
	}

	int size = mButtons.size();
	for (int i = 0;i<size; i++)
	{
		if (IsEnable())
		{
			CButton* btn = mButtons[i];
			btn->SetEnable(true);
			btn->SetPos(mPosition + BACK_ADJUST_POS + CVector2(0.0f, 30.0f * i));
			btn->Update();
		}
	}
}

// 描画
void CItemMenu::Render()
{
	mpBack->Render();
	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// 待機状態
void CItemMenu::UpdateIdle()
{
}

// メニューを開く
void CItemMenu::UpdateOpen()
{
}

// メニュー選択
void CItemMenu::UpdateSelect()
{
}

// 状態切り替え
void CItemMenu::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
}

// [Use]クリック時のコールバック関数
void CItemMenu::OnClickUse()
{
}

// [Back]クリック時のコールバック関数
void CItemMenu::OnClickBack()
{
}
