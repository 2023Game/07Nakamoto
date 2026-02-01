#include "CResultScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CCamera.h"
#include "CResultUI.h"
#include "CBGMManager.h"
#include "CItemManager.h"
#include "CCrystalManager.h"
#include "CGameData.h"

// コンストラクタ
CResultScene::CResultScene()
	:CSceneBase(EScene::eResult)
{
}

// デストラクタ
CResultScene::~CResultScene()
{
}

//シーン読み込み
void CResultScene::Load()
{
	// タイトルBGMを再生
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpResultUI = new CResultUI();
	AddTask(mpResultUI);
}

//シーンの更新処理
void CResultScene::Update()
{
	// ゲームクリア画面が
	if (mpResultUI->IsEnd())
	{
		// コンティニューならば、ゲームシーンを読み込む
		if (mpResultUI->IsContinue())
		{
			// アイテムマネージャーを破棄
			CItemManager::Instance()->Destroy();
			// クリスタルマネージャーを破棄
			CCrystalManager::Instance()->Destroy();

			// ゲームが終了したので、ゲームデータをリセット
			CGameData::Reset();

			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// タイトルならば、タイトルシーンを読み込む
		else if (mpResultUI->IsTitle())
		{
			// ゲームが終了したので、ゲームデータをリセット
			CGameData::Reset();

			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
	}
}
