#include "CItemMenu.h"

CItemMenu* CItemMenu::spInstance = nullptr;

// インスタンスを取得
CItemMenu* CItemMenu::Instance()
{
	return spInstance;
}

// コンストラクト
CItemMenu::CItemMenu()
	: mIsOpened(false)
{
	spInstance = this;

	// メニューリストを作成
	mMenuList =
	{
		{"Use" , EContent::eUse},
		{"Back", EContent::eBack},
	};
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
	
}

// 描画
void CItemMenu::Render()
{

}
