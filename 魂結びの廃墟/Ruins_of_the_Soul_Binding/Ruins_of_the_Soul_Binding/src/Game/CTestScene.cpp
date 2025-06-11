#include "CTestScene.h"
#include "CSceneManager.h"
#include "CTestField.h"
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
#include "CNavManager.h"]
#include "CInteractObjectManager.h"

//コンストラクタ
CTestScene::CTestScene()
	: CSceneBase(EScene::eTest)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CTestScene::~CTestScene()
{
}

//シーン読み込み
void CTestScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	CResourceManager::Load<CModel>(		"Field",			"Field\\field.obj");
	CResourceManager::Load<CModel>(		"FieldCube",		"Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>(		"FieldCylinder",	"Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>(	"Player",			"Character\\Player\\player.x");
	CResourceManager::Load<CModelX>(	"Cactus",			"Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModel>(		"CactusNeedle",		"Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>(	"Laser",			"Effect\\laser.png");
	CResourceManager::Load<CTexture>(	"LightningBolt",	"Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>(		"Slash",			"Effect\\slash.obj");
	CResourceManager::Load<CSound>(		"SlashSound",		"Sound\\SE\\slash.wav");
	CResourceManager::Load<CModel>(		"Sword",			"Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>(		"Shield",			"Weapon\\Shield\\shield.obj");

	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk\\idle.x");
	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");

	//CResourceManager::Load<CModel>(	"Floor",		"Field\\Abandoned_School_Floor.obj");
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Test\\test_wall2.obj");
	CResourceManager::Load<CModel>(		"WallCol",			 "Field\\Test\\test_wall_col.obj");
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		 "Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			 "Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		 "Door\\left_door_col.obj");
	CResourceManager::Load<CModel>(		"DemonPower",		 "Field\\Object\\Demon_Power\\source_of_demon_power.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 調べるオブジェクトの管理クラスの作成
	new CInteractObjectManager();

	// プレイヤーの管理クラスの作成
	new CPlayerManager();

	// 経路探索管理クラスを作成
	new CNavManager();

	// フィールドの生成
	mpField = new CTestField();

	// サボテンの敵を作成
	//CCactus* cactus = new CCactus();
	//cactus->Position(0.0f, 0.0f, -100.0f);

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);

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
			CVector(70.0f, 1.0, 60.0f),
			CVector(-70.0f, 1.0, 60.0f),
			CVector(-70.0f, 1.0, 120.0f),
			CVector(70.0f, 1.0, 120.0f),
		}
	);
	warrok->Scale(1.0f, 1.0f, 1.0f);
	warrok->Position(70.0f, 1.0, 120.0f);

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

	// 猫視点のカメラを作成
	CCatCamera* catCamera = new CCatCamera(cat);
	cat->SetCamera(catCamera);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CTestScene::Update()
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
