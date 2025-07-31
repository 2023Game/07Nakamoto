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
#include "CGameUI.h"
#include "CSpider.h"
#include "CInventory.h"
#include "CRoomManager.h"

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

	// 次に読み込まれるシーンがゲームシーンでなければ、
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr)
	{
		// アイテムのリソースを全て破棄する
		Item::DeleteItemResources();

		// ゲームUIを破棄する
		CGameUI::ClearInstance();
	}
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
	// プレイヤー
	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk2\\idle.x");
	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");
	// 敵
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");
	CResourceManager::Load<CModelX>(	"Spider",			"Character\\Enemy\\Spider\\spider.x");
	// 床
	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	// 壁
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall3.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col3.obj");
	// 扉
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		"Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			"Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		"Door\\left_door_col.obj");
	CResourceManager::Load<CModel>(		"OpeningDoor",		"Door\\opening_door.obj");
	CResourceManager::Load<CModel>(		"OpeningDoorCol",	"Door\\opening_door_col.obj");
	// 妖力の源
	CResourceManager::Load<CModel>(		"DemonPower",		"Field\\Object\\Demon_Power\\source_of_demon_power.obj");
	// スイッチ
	CResourceManager::Load<CModel>(		"Stand",			"Field\\Object\\Switch\\stand.obj");
	CResourceManager::Load<CModel>(		"Button",			"Field\\Object\\Switch\\buttun.obj");
	CResourceManager::Load<CModel>(		"StandWallCol",		"Field\\Object\\Switch\\stand_wall_col.obj");
	CResourceManager::Load<CModel>(		"StandFloorCol",	"Field\\Object\\Switch\\stand_floor_col.obj");
	// 火球
	CResourceManager::Load<CModel>(		"Fireball",			"Weapon\\fire.obj");
	

	// テスト用の移動エリアを制限する壁
	CResourceManager::Load<CModel>(		"TestObj",			"Field\\Test\\obj4.obj");
	CResourceManager::Load<CModel>(		"TestObjFloor",		"Field\\Test\\obj4_floor.obj");
	CResourceManager::Load<CModel>(		"TestObjWall",		"Field\\Test\\obj4_wall.obj");

	CResourceManager::Load<CModel>(		"TestWall",			"Field\\Test\\test_wall0.obj");
	CResourceManager::Load<CModel>(		"TestWallBrack",	"Field\\Test\\Brack_Wall.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 部屋管理クラスを生成
	new CRoomManager();

	// 調べるオブジェクトの管理クラスの生成
	new CInteractObjectManager();

	// プレイヤーの管理クラスの生成
	new CPlayerManager();

	// 経路探索管理クラスを生成
	new CNavManager();

	// フィールドの生成
	mpField = new CField();

	// ゲームメニューを生成
	new CInventory();

	// UIの生成
	new CGameUI();

	// プレイヤーの生成
	CPlayer2* player2 = new CPlayer2();
	player2->Scale(1.0f, 1.0f, 1.0f);

	// 猫の生成
	CCat* cat = new CCat();
	cat->Scale(1.0f, 1.0f, 1.0f);
	cat->Position(10.0f, 0.0f, 0.0f);

	// ウォーロックの生成
	CBoss* warrok = new CBoss
	(
		{
			// ルート1
			{
				CVector(200.0f, 0.0, 60.0f),
				CVector(200.0f, 0.0, 180.0f),
				CVector(120.0f, 0.0, 180.0f),
				CVector(120.0f, 0.0, 450.0f),
				CVector(-20.0f, 0.0, 450.0f),
				CVector(-20.0f, 0.0, 395.0f),
			},
			// ルート2
			{
				CVector(-300.0f, 0.0, 395.0f),
				CVector(-300.0f, 0.0, 256.5f),
				CVector(-210.0f, 0.0, 256.5f),
				CVector(-210.0f, 0.0, 55.5f),
				CVector(-274.5f, 0.0, 55.5f),
				CVector(-274.5f, 0.0, -210.0f),
				CVector(-155.0f, 0.0, -210.0f),
				CVector(-155.0f, 0.0, -300.0f),
				CVector(-20.0f, 0.0, -300.0f),
			},
			// ルート3
			{
				CVector(85.0f, 0.0, -250.0f),
				CVector(85.0f, 0.0, -64.5f),
				CVector(66.0f, 0.0, -64.5f),
				CVector(66.0f, 0.0, 115.0f),
				CVector(-50.0f, 0.0, 115.0f),
				CVector(-90.0f, 0.0, 180.0f),
				CVector(-110.0f, 0.0, 194.5f),
				CVector(-170.0f, 0.0, 194.5f),

			},
			// ルート4
			{
				CVector(-11.5f, 0.0, -180.0f),
				CVector(-155.0f, 0.0, -180.0f),
				CVector(65.0f, 0.0, -60.0f),
				CVector(65.0f, 0.0, 60.0f),
				CVector(-90.0f, 0.0, 60.0f),
				CVector(-90.0f, 0.0, 310.0f),
				CVector(-80.0f, 0.0, 330.0f),
			},
			// ルート5
			{
				CVector(-90.0f, 0.0, 395.0f),
				CVector(-265.0f, 0.0, 395.0f),
				CVector(-265.0f, 0.0, 256.5f),
				CVector(-230.0f, 0.0, 256.5f),
				CVector(-180.0f, 0.0, 195.0f),
				CVector(-90.0f, 0.0, 180.0f),
				CVector(66.0f, 0.0, 60.0f),
				CVector(66.0f, 0.0, -62.0f),
				CVector(90.0f, 0.0, -65.0f),
				CVector(90.0f, 0.0, -130.0f),
				CVector(5.0f, 0.0, -130.0f),
				CVector(-10.0f, 0.0, -150.0f),
			},

		}
	);
	warrok->Scale(1.0f, 1.0f, 1.0f);
	warrok->Position(90.0f, 1.0, 60.0f);

	//// 蜘蛛
	//CSpider* spider = new CSpider
	//(
	//	{
	//		CVector(100.0f,0.0f,50.0f),
	//		CVector(100.0f,0.0f, 50.0f),
	//	}
	//);
	//spider->Scale(10.0f, 10.0f, 10.0f);
	//spider->Position(100.0f, 0.0f, 50.0f);

	//// ウォーロックの生成
	//CBoss* warrok1 = new CBoss
	//(
	//	{
	//		CVector(-20.0f, 0.0, 380.0f),
	//		CVector(-90.0f, 0.0, 380.0f),
	//		CVector(-90.0f, 0.0, 320.0f),
	//		CVector(-20.0f, 0.0, 320.0f),
	//	}
	//);
	//warrok1->Scale(1.0f, 1.0f, 1.0f);
	//warrok1->Position(-20.0f, 1.0, 400.0f);

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

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	CInventory* inv = CInventory::Instance();
	if (CInput::PushKey(VK_TAB))
	{
		if (!inv->IsOpened())
		{
			inv->Open();
		}
		else
		{
			inv->Close();
		}
	}

#if _DEBUG
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
#endif
}
