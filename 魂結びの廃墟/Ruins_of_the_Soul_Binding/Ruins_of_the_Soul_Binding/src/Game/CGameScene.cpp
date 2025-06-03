#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CPlayer2.h"
#include "CCat.h"
#include "CPlayerManager.h"
#include "CGameCamera.h"
#include "CPlayerCamera.h"
#include "CCatCamera.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCactus.h"
#include "CWarrok.h"
#include "CNavManager.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う
	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk\\idle.x");
	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");
	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col.obj");
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		"Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			"Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		"Door\\left_door_col.obj");
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// プレイヤーの管理クラスの作成
	new CPlayerManager();

	// 経路探索管理クラスを作成
	new CNavManager();

	// フィールドの生成
	mpField = new CField();

	// プレイヤーの生成
	CPlayer2* player2 = new CPlayer2();
	player2->Scale(1.0f, 1.0f, 1.0f);

	// 猫の生成
	CCat* cat = new CCat();
	cat->Scale(1.0f, 1.0f, 1.0f);
	cat->Position(10.0f, 0.0f, 0.0f);

	// ウォーロックの生成
	CWarrok* warrok = new CWarrok
	(
		{
			CVector( 70.0f, 1.0, 150.0f),
			CVector(-70.0f, 1.0, 150.0f),
			CVector(  0.0f, 1.0, 100.0f),
		}
	);
	warrok->Scale(1.0f, 1.0f, 1.0f);
	warrok->Position(70.0f, 1.0, 150.0f);

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CPlayerCameraのテスト
	CVector atPos = player2->Position() + CVector(0.0f, 10.0f, 0.0f);
	CPlayerCamera* mainCamera = new CPlayerCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(player2);
	mainCamera->AddCollider(mpField->GetFloorCol());
	mainCamera->AddCollider(mpField->GetWallCol());
	player2->SetCamera(mainCamera);
	player2->SetOperate(true);

	atPos = cat->Position() + CVector(0.0f, 2.5f, 0.0f);
	CCatCamera* catCamera = new CCatCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	catCamera->SetFollowTargetTf(cat);
	catCamera->AddCollider(mpField->GetFloorCol());
	catCamera->AddCollider(mpField->GetWallCol());
	cat->SetCamera(catCamera);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CGameScene::Update()
{
	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

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
