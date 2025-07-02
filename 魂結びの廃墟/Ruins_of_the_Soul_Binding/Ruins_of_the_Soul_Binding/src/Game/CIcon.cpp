#include "CIcon.h"
#include "CImage.h"

#define ICON_CAT_PATH "UI\\Silhouette\\cat.png"
#define ICON_GIRL_PATH "UI\\Silhouette\\girl.png"
#define ICON_CAT_GIRL_PATH "UI\\Silhouette\\cat_girl.png"

#define ICON_FRAME_PATH "UI\\Silhouette\\frame.png"

// アイコンのインスタンスの変数定義
CIcon* CIcon::spInstance = nullptr;

// インスタンスの取得
CIcon* CIcon::Instance()
{
	return spInstance;
}

CIcon::CIcon()
	: mpIcon(nullptr)
	, mpIconFrame(nullptr)
{
	spInstance = this;

	// アイコンを生成
	mpIcon = new CImage
	(
		ICON_CAT_GIRL_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIcon->SetPos(CVector2(0.0f,40.0f));

	// アイコンのフレーム
	mpIconFrame = new CImage
	(
		ICON_FRAME_PATH,
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eGame,
		false,
		false
	);
	mpIconFrame->SetPos(CVector2(0.0f, 40.0f));
}

// デストラクタ
CIcon::~CIcon()
{
	SAFE_DELETE(mpIcon);
	SAFE_DELETE(mpIconFrame);
}

// アイコンの設定
void CIcon::SetIcon(int icon)
{
	if (icon == (int)EIcon::eCat)
	{
		mpIcon->Load(ICON_CAT_PATH);
	}
	else if (icon == (int)EIcon::ePlayer)
	{
		mpIcon->Load(ICON_GIRL_PATH);
	}
	else if (icon == (int)EIcon::eTogether)
	{
		mpIcon->Load(ICON_CAT_GIRL_PATH);
	}

}

// 更新
void CIcon::Update()
{
}

// 描画
void CIcon::Render()
{
	// アイコンの描画
	mpIcon->Render();
	// アイコンのフレーム描画
	mpIconFrame->Render();
}

