#include "CItemMenu.h"
#include "CImage.h"
#include "CText.h"
#include "CItemSlotUI.h"

#define BACK_SIZE 60.0f

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
	, mpBack(nullptr)
	, mpText(nullptr)
	, mIsOpened(false)
{
	spInstance = this;

	// メニューリストを作成
	mMenuList =
	{
		{"Use" , EContent::eUse},
		{"Back", EContent::eBack},
	};

	// 文字描画の背景
	mpBack = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eMenu,
		false, false
	);
	mpBack->SetSize(BACK_SIZE, BACK_SIZE);
	mpBack->SetAlpha(0.5f);
	mpBack->SetEnable(false);
}

// デストラクタ
CItemMenu::~CItemMenu()
{
	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// 開く
void CItemMenu::Open()
{
	if (mIsOpened) return;

	mIsOpened = true;
}

// 閉じる
void CItemMenu::Close()
{
	if (!mIsOpened) return;

	mIsOpened = false;
}

// インベントリが開いているかどうか
bool CItemMenu::IsOpened() const
{
	return mIsOpened;
}

// 更新
void CItemMenu::Update()
{
	mpBack->Update();
}

// 描画
void CItemMenu::Render()
{
	if (mpBack->IsEnable())
	{
		mpBack->Render();
	}
	
}
