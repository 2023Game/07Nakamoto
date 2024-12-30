#include "CGameScene3.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer2.h"
#include "CEnemy1.h"
#include "CEnemy2.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"

// コンストラクタ
CGameScene3::CGameScene3()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

// デストラクタ
CGameScene3::~CGameScene3()
{
}

// シーン読み込み
void CGameScene3::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う
	CResourceManager::Load<CModel>("Field",			"Field\\stage1\\floor.obj");
	CResourceManager::Load<CModel>("Goal",			"Field\\stage1\\goal.obj");
	CResourceManager::Load<CModel>("Outer_Wall",	"Field\\stage1\\outer_wall.obj");
	CResourceManager::Load<CModel>("Wall",			"Field\\stage1\\wall.obj");
	CResourceManager::Load<CModel>("Gimmick_Wall",	"Field\\stage1\\gimmick_wall.obj");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 経路探索管理クラスを作成
	new CNavManager();
	// フィールド作成
	new CField();

	// プレイヤー生成
	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	//pico->Position(0.0f, 0.0f, 0.0f);

	//// 敵①生成
	//CEnemy2* enemy2 = new CEnemy2
	//(
	//	{
	//		CVector(70.0f, 1.0, 10.0f),
	//		CVector(180.0f, 1.0, 10.0f),
	//		CVector(180.0f, 1.0, 100.0f),
	//		CVector(80.0f, 1.0, 100.0f),
	//	}
	//);
	//enemy2->Scale(1.0f, 1.0f, 1.0f);
	//enemy2->Position(CVector(70.0f, 1.0, 10.0f));

	// CGameCamera2のテスト
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

// シーン更新処理
void CGameScene3::Update()
{
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}
}
