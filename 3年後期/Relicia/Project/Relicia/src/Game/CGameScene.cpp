#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CAdventurer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCactus.h"
#include "CMashroom.h"
#include "CElementManager.h"
#include "CInteractObjectManager.h"
#include "CInventory.h"
#include "CNavManager.h"
#include "CDebugInput.h"
#include "CPlayer.h"
#include "CItemManager.h"
#include "CCrystalManager.h"
#include "CGameData.h"
#include "CBspMap.h"
#include "Maths.h"
#include "CEnemyManager.h"


//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
	CInteractObjectManager::ClearInstance();
	CItemManager::ClearInstance();
	CCrystalManager::ClearInstance();
	CEnemyManager::ClearInstance();
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

	CResourceManager::Load<CModel>(		"Door",				"Dungeon\\Door.obj");
	CResourceManager::Load<CModel>(		"Floor",			"Dungeon\\Floor.obj");
	CResourceManager::Load<CModel>(		"Pillar",			"Dungeon\\Pillar.obj");
	CResourceManager::Load<CModel>(		"Pillar_Col",		"Dungeon\\Pillar_Col.obj");
	CResourceManager::Load<CModel>(		"Wall",				"Dungeon\\Wall.obj");
	CResourceManager::Load<CModel>(		"Wall_Entrance",	"Dungeon\\Wall_Entrance.obj");
	CResourceManager::Load<CModel>(		"Entrance_Col",		"Dungeon\\Entrance_Wall_Col.obj");
	CResourceManager::Load<CModel>(		"Entrance_Ceil_Col","Dungeon\\Entrance_Ceil_Col.obj");

	CResourceManager::Load<CModelX>(	"Bat",				"Character\\Enemy\\Bat\\bat.x");
	CResourceManager::Load<CModelX>(	"Mushroom",			"Character\\Enemy\\Mushroom\\mushroom.x");

	CResourceManager::Load<CModel>(		"Crate",			"Field\\Object\\crate.obj");
	CResourceManager::Load<CModel>(		"Crystal",			"Field\\Object\\Crystal\\red_crystal.obj");
	CResourceManager::Load<CModel>(		"Key",				"Field\\Object\\key.obj");

	CResourceManager::Load<CModel>(		"Escape",			"Field\\Object\\escape.obj");

	CResourceManager::Load<CModel>(		"Wall2",				"Dungeon\\Wall2.obj");
	CResourceManager::Load<CModel>(		"Wall_Entrance2",		"Dungeon\\Wall_Entrance2.obj");
	CResourceManager::Load<CModel>(		"Entrance_Col2",		"Dungeon\\Entrance_Wall_Col2.obj");
	CResourceManager::Load<CModel>(		"Entrance_Ceil_Col2",	"Dungeon\\Entrance_Ceil_Col2.obj");

	CResourceManager::Load<CModel>(		"Switch_Floor",			"Gimmick\\switch_floor.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 調べるオブジェクトの管理クラスの生成
	new CInteractObjectManager();

	// 経路探索管理クラスを生成
	new CNavManager();

	// ダンジョンの作成
	mpField = new CField();

	//new CField2(0);

	// インベントリ画面の生成
	new CInventory();

	// 属性の管理クラスを生成
	new CElementManager();

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);
	//player->Position(mpField->GetRandomFloorPos());

	// プレイヤーの作成
	CAdventurer* player = new CAdventurer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(mpField->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Player));

	// 敵の生成前に初期化しておく
	CEnemyManager::Instance()->AllClear();
	// 敵の生成
	CEnemyManager::Instance()->CreateEnemys();

	// CGameCameraのテスト
	//CGameCamera* mainCam
	// era = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(player);
	mainCamera->AddCollider(mpField->GetFloorCollider());
	mainCamera->AddCollider(mpField->GetWallCollider());

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
	if (CDebugInput::PushKey('H'))
	{
		CEnemyManager::Instance()->RandomEnemySpawn();

		//CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CDebugInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	if (CDebugInput::PushKey('B'))
	{
		// プレイヤーの座標を再設定
		CAdventurer::Instance()->Position(mpField->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Player));
		// 敵の生成
		CEnemyManager::Instance()->CreateEnemys();
	}

	CDebugPrint::Print("階層：%d階\n", CGameData::floorNum);
#endif
}
