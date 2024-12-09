#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CPlayer2.h"
#include "CEnemy.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CEnemy2.h"

#include "CTouch.h"
#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "CDoor.h"

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

	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModel>("Wall",		"Field\\Object\\Wall\\Wall.obj");
	CResourceManager::Load<CModel>("WallCol",	"Field\\Object\\Wall\\WallCol.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CTexture>("LightningBolt", "Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\mutant\\mutant.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");

	CResourceManager::Load<CModel>("Switch", "Object\\Switch.obj");
	CResourceManager::Load<CModel>("Touch", "Object\\Touch.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// 経路探索管理クラスを作成
	new CNavManager();

	new CPushSwitchManager();

	new CField();

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);
	//player->Position(-50.0f, 1.0f, 0.0f);

	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(-50.0f, 1.0f, 10.0f);

	// 敵①生成
	//CEnemy* enemy = new CEnemy
	//(
	//	{
	//		CVector(100.0f, 1.0,   0.0f),
	//		CVector(  0.0f, 1.0,   0.0f),
	//		CVector(  0.0f, 1.0, 100.0f),
	//		CVector(100.0f, 1.0, 100.0f),
	//	}
	//);
	//enemy->Scale(1.0f, 1.0f, 1.0f);
	//enemy->Position(CVector(200.0f, 1.0, 0.0f));
	// 敵②生成
	CEnemy2* enemy2 = new CEnemy2
	(
		{
			CVector(250.0f, 1.0, 150.0f),
			CVector(150.0f, 1.0, 150.0f),
			CVector(150.0f, 1.0, 250.0f),
			CVector(250.0f, 1.0, 250.0f),
		}
	);
	enemy2->Scale(1.0f, 1.0f, 1.0f);
	enemy2->Position(CVector(200.0f, 1.0, 200.0f));


	// 松明(仮)の描画の確認	削除予定
	CTouch* touch = new CTouch(CVector(0.0f, 0.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f), CVector(0.3f, 0.3f, 0.3f));

	// スイッチを生成
	CPushSwitch* push_switch = new CPushSwitch(CVector(-30.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f,1.0f,1.0f));

	// スイッチを生成
	CPushSwitch* push_switch2 = new CPushSwitch(CVector(-50.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));


	// 扉を生成
	//CDoor(CPushSwitchManager::Instance()->)

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
