#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"
#include "CInventory.h"
#include "CButton.h"

#define BACK_ADJUST_POS CVector2(30.0f,-30.0f)	//背景の調整位置
#define BUTTON_SIZE CVector2(60.0f,30.0f)		// ボタンのサイズ
#define BUTTON_POS_Y 30.0f		// ボタンのY座標

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
	, mDecision(false)
	, mSelectIndex(-1)
{
	spInstance = this;

	// 使うボタンを生成
	CButton* btn1 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI\\ItemMenu\\use1.png", "UI\\ItemMenu\\use2.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CItemMenu::OnClickUse, this));
	// ボタンリストに追加
	mButtons.push_back(btn1);

	// 装備ボタンを生成
	CButton* btn2 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn2->LoadButtonImage("UI\\ItemMenu\\equip1.png", "UI\\ItemMenu\\equip2.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn2->SetOnClickFunc(std::bind(&CItemMenu::OnClickUse, this));
	// ボタンリストに追加
	mButtons.push_back(btn2);

	// 戻るボタンを生成
	CButton* btn3 = new CButton
	(
		CVector2::zero,
		BUTTON_SIZE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	btn3->LoadButtonImage("UI\\ItemMenu\\close1.png", "UI\\ItemMenu\\close2.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn3->SetOnClickFunc(std::bind(&CItemMenu::OnClickClose, this));
	// ボタンリストに追加
	mButtons.push_back(btn3);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// 表示する
void CItemMenu::Open()
{
	SetEnable(true);
	SetShow(true);
	mSelectIndex = -1;
	mDecision = false;

}

// 閉じる
void CItemMenu::Close()
{
	SetEnable(false);
	SetShow(false);
	mSelectIndex = -1;
	mDecision = false;

}

// アイテムを使うか
bool CItemMenu::IsUse() const
{
	return mSelectIndex == 0;
}

bool CItemMenu::IsEquipment() const
{
	return mSelectIndex == 1;
}

// アイテムメニューを閉じるか
bool CItemMenu::IsClose() const
{
	return mSelectIndex == 2;
}

// 更新
void CItemMenu::Update()
{
	switch (mState)
	{
	case EState::eIdle:
		UpdateIdle();
		break;
	case EState::eSelect:
		UpdateSelect();
		break;
	case EState::eUse:

		break;
	case EState::eBack:

		break;
	}

	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// 描画
void CItemMenu::Render()
{
	// メニューボタンを表示
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// 待機状態
void CItemMenu::UpdateIdle()
{
	// ボタンの位置の設定
	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		btn->SetPos(mPosition + BACK_ADJUST_POS + CVector2(0.0f, BUTTON_POS_Y * i));
	}
	//ChangeState(EState::eSelect);
}

// メニュー選択
void CItemMenu::UpdateSelect()
{
	ChangeState(EState::eIdle);
}

// 状態切り替え
void CItemMenu::ChangeState(EState state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数の初期化
	mState = state;
	mStateStep = 0;
}

// [使う]クリック時のコールバック関数
void CItemMenu::OnClickUse()
{
	if (mDecision) return;

	mSelectIndex = 0;
	mDecision = true;
}

// [装備]クリック時のコールバック関数
void CItemMenu::OnClickEquipment()
{
	if (mDecision) return;

	mSelectIndex = 1;
	mDecision = true;
}

// [戻る]クリック時のコールバック関数
void CItemMenu::OnClickClose()
{
	if (mDecision) return;

	mSelectIndex = 2;
	mDecision = true;
}
