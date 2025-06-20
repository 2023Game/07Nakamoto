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
#include "CBoss.h"
#include "CNavManager.h"
#include "CInteractObjectManager.h"
#include "CDemonPowerManager.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
	// 調べるオブジェクトの管理クラスを削除
	CInteractObjectManager::CIearInstance();
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
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");
	// 床
	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	// 壁
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col.obj");
	// 扉
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		"Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			"Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		"Door\\left_door_col.obj");
	// 妖力の源
	CResourceManager::Load<CModel>(		"DemonPower",		"Field\\Object\\Demon_Power\\source_of_demon_power.obj");
	// スイッチ
	CResourceManager::Load<CModel>(		"Stand",			"Field\\Object\\Switch\\stand.obj");
	CResourceManager::Load<CModel>(		"Button",			"Field\\Object\\Switch\\buttun.obj");
	CResourceManager::Load<CModel>(		"StandCol",			"Field\\Object\\Switch\\stand_col.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 調べるオブジェクトの管理クラスの作成
	new CInteractObjectManager();

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

	//// ウォーロックの生成
	//CBoss* warrok = new CBoss
	//(
	//	{
	//		CVector( 70.0f, 1.0, 150.0f),
	//		CVector(-70.0f, 1.0, 150.0f),
	//		CVector(  0.0f, 1.0, 100.0f),
	//	}
	//);
	//warrok->Scale(1.0f, 1.0f, 1.0f);
	//warrok->Position(70.0f, 1.0, 150.0f);

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CPlayerCameraのテスト
	CVector atPos = player2->Position() + CVector(0.0f, 10.0f, 0.0f);
	CPlayerCamera* playerCamera = new CPlayerCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	playerCamera->SetFollowTargetTf(player2);
	playerCamera->AddCollider(mpField->GetFloorCol());
	playerCamera->AddCollider(mpField->GetWallCol());
	player2->SetCamera(playerCamera);
	player2->SetOperate(true);

	// 猫視点のカメラを作成
	CCatCamera* catCamera = new CCatCamera(cat);
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
