#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CPlayer2.h"
#include "CEnemy1.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
//#include "CGameMenu.h"
#include "CInventory.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CCactus.h"

#include "CEnemy2.h"
#include "CWarrok.h"

#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "CSwitchDoor.h"

#include "CTouch.h"
#include "CPlayer3.h"
#include "CGhost.h"

#include "ItemData.h"
#include "CChoco.h"
#include "CCookie.h"
#include "CStone.h"


//コンストラクタ
CGameScene::CGameScene()
	: CGameSceneBase(EScene::eTestGame)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
	// 次に読み込まれるシーンがゲームシーンでなければ、
	//アイテムのリソースを全て破棄する
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
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
	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModel>("Wall",		"Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>("WallCol",	"Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CModelX>("Cactus", "Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModel>("CactusNeedle", "Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");
	CResourceManager::Load<CModel>("Sword", "Weapon\\Sword\\sword.obj");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\mutant\\mutant.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");
	CResourceManager::Load<CModel>("Switch_OFF", "Object\\Switch\\switch_Off.obj");
	CResourceManager::Load<CModel>("Switch_ON", "Object\\Switch\\switch_On.obj");

	CResourceManager::Load<CModel>("Spider_Web", "Character\\Enemy\\trap\\spider_web.obj");

	CResourceManager::Load<CModel>("Touch", "Object\\Touch.obj");

	CResourceManager::Load<CModelX>("Player3", "Character\\Player3\\Acquire.x");
	CResourceManager::Load<CModelX>("Ghost", "Character\\Enemy\\Ghost\\ghost.x");
	CResourceManager::Load<CModel>("Tourou", "Object\\Lantern\\stone_lantern.obj");

	CResourceManager::Load<CModelX>("Warrok", "Character\\Enemy\\warrok2\\warrok.x");

	//シーン読み込み
	CGameSceneBase::Load();

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// サボテンの敵を作成
	CCactus* cactus = new CCactus();
	cactus->Position(0.0f, 0.0f, -100.0f);

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);
	//player->Position(-50.0f, 1.0f, 0.0f);

	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(-50.0f, 0.0f, 10.0f);

	//CPlayer3* player3 = new CPlayer3();
	//player3->Scale(1.0f, 1.0f, 1.0f);
	//player3->Position(-50.0f, 0.0f, -10.0f);

	// 敵�@生成
	CEnemy1* enemy = new CEnemy1
	(
		{
			CVector(100.0f, 0.0f,   0.0f),
			CVector(  0.0f, 0.0f,   0.0f),
			CVector(  0.0f, 0.0f, 100.0f),
			CVector(100.0f, 0.0f, 100.0f),
		}
	);
	enemy->Scale(1.0f, 1.0f, 1.0f);
	enemy->Position(CVector(200.0f, 1.0, 0.0f));
	// 敵�A生成
	CWarrok* enemy2 = new CWarrok
	(
		{
			CVector(250.0f, 0.0f, 150.0f),
			CVector(150.0f, 0.0f, 150.0f),
			CVector(150.0f, 0.0f, 250.0f),
			CVector(250.0f, 0.0f, 250.0f),
		}
	);
	enemy2->Scale(1.0f, 1.0f, 1.0f);
	enemy2->Position(CVector(200.0f, 0.0, 200.0f));

	// 幽霊の敵を作成
	CGhost* ghost = new CGhost(
		{
			CVector(-50.0f, 0.0f, -100.0f),
			CVector(0.0f, 0.0f, -150.0f),
			CVector(50.0f, 0.0f, -100.0f),
		}
	);
	ghost->Scale(1.0f, 1.0f, 1.0f);
	ghost->Position(-50.0f, 0.0f, -100.0f);


	// 松明(仮)の描画の確認	削除予定
	CTouch* touch = new CTouch(CVector(0.0f, 0.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	// チョコを生成
	CChoco* choco = new CChoco();
	choco->Position(0.0f, 10.0f, 20.0f);

	choco = new CChoco();
	choco->Position(-100.0f, 10.0f, 20.0f);
	choco = new CChoco();
	choco->Position(-100.0f, 10.0f, 70.0f);
	choco = new CChoco();
	choco->Position(-100.0f, 10.0f, 120.0f);
	choco = new CChoco();
	choco->Position(-100.0f, 10.0f, 170.0f);
	choco = new CChoco();
	choco->Position(-100.0f, 10.0f, 220.0f);

	CCookie* cookie = new CCookie();
	cookie->Position(-150.0f, 10.0f, 20.0f);
	cookie = new CCookie();
	cookie->Position(-150.0f, 10.0f, 70.0f);
	CStone* stone = new CStone();
	stone->Position(-200.0f, 10.0f, 20.0f);
	stone = new CStone();
	stone->Position(-200.0f, 10.0f, 70.0f);

	// スイッチを生成
	CPushSwitch* push_switch = new CPushSwitch(CVector(-30.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f,1.0f,1.0f));
#if _DEBUG
	push_switch->SetDebugName("Switch1");
#endif

	// スイッチを生成
	CPushSwitch* push_switch2 = new CPushSwitch(CVector(-50.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch2->SetDebugName("Switch2");
#endif

	// スイッチで開く扉�@を作成
	CSwitchDoor* door = new CSwitchDoor
	(
		CVector(-50.0f, 0.0f, -50.0f),
		CVector( 0.0f,  0.0f,   0.0f),
		CVector( 2.0f,  5.0f,   2.0f)
	);
	door->SetAnimPos
	(
		CVector(-30.0f, 0.0f, -50.0f),	// 扉を開いた時の座標
		CVector(-50.0f, 0.0f, -50.0f)	// 扉を閉じた時の座標
	);
	door->AddSwitch(push_switch);		// 押したら開くスイッチを追加

	// スイッチで開く扉�Aを作成
	CSwitchDoor* door2 = new CSwitchDoor
	(
		CVector(-120.0f, 0.0f, -50.0f),
		CVector(   0.0f, 0.0f,   0.0f),
		CVector(   2.0f, 5.0f,   2.0f)
	);
	door2->SetAnimPos
	(
		CVector(-100.0f, 0.0f, -50.0f),	// 扉を開いた時の座標
		CVector(-120.0f, 0.0f, -50.0f)	// 扉を閉じた時の座標
	);
	door2->AddSwitch(push_switch);		// 押したら開くスイッチを追加
	door2->AddSwitch(push_switch2);		// 押したら開くスイッチを追加

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2のテスト
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);
}

//シーンの更新処理
void CGameScene::Update()
{
	CGameSceneBase::Update();

	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	//if (CInput::PushKey('H'))
	//{
	//	CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	//}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	//CInventory* inv = CInventory::Instance();
	//if (CInput::PushKey(VK_TAB))
	//{
	//	if (!inv->IsOpened())
	//	{
	//		inv->Open();
	//	}
	//	else
	//	{
	//		inv->Close();
	//	}
	//}
}
