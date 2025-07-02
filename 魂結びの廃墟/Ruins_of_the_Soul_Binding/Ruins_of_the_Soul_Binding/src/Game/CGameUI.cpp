#include "CGameUI.h"
#include "CInteractUI.h"
#include "CInteractObject.h"
#include "CIcon.h"

// CGameUIのインスタンスの変数定義
CGameUI* CGameUI::spInstance = nullptr;

// インスタンスを取得する
CGameUI* CGameUI::Instance()
{
	return spInstance;
}

// インスタンスを破棄する
void CGameUI::ClearInstance()
{
	SAFE_DELETE(spInstance);
}

// コンストラクタ
CGameUI::CGameUI()
	:mpInteractUI(nullptr)
{
	if (spInstance != nullptr)
	{
		SAFE_DELETE(spInstance);
	}
	spInstance = this;

	mpInteractUI = new CInteractUI();

	mpIcon = new CIcon();
}

// デストラクタ
CGameUI::~CGameUI()
{
	SAFE_DELETE(mpIcon);
}

// 指定したオブジェクトの調べるUIを表示
void CGameUI::ShowInteractUI(CInteractObject* obj)
{
	mpInteractUI->Show(obj);
}

// 調べるUIを非表示にする
void CGameUI::HideInteractUI()
{
	mpInteractUI->Hide();
}
