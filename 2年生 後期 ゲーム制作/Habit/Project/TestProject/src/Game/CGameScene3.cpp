#include "CGameScene3.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer2.h"
#include "CEnemy1.h"
#include "CEnemy2.h"
#include "CWarrok.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
//#include "CGameMenu.h"
#include "CInventory.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "CSwitchDoor.h"
#include "CClearArea.h"
#include "ItemData.h"

// コンストラクタ
CGameScene3::CGameScene3()
	: CGameSceneBase(EScene::eGame1)
{
}

// デストラクタ
CGameScene3::~CGameScene3()
{
	// 次に読み込まれるシーンがゲームシーンでなければ、
	// アイテムのリソースを全て破棄する
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
	}
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
	CResourceManager::Load<CModel>("Field", "Field\\stage1\\floor.obj");
	CResourceManager::Load<CModel>("Wall", "Field\\stage1\\wall.obj");
	CResourceManager::Load<CModel>("Wall_Col", "Field\\stage1\\wall_col.obj");
	CResourceManager::Load<CModel>("Gimmick_Wall", "Field\\stage1\\gimmick_wall.obj");
	CResourceManager::Load<CModel>("Switch_OFF", "Object\\Switch\\switch_Off.obj");
	CResourceManager::Load<CModel>("Switch_ON", "Object\\Switch\\switch_On.obj");
	CResourceManager::Load<CModel>("Goal", "Object\\clear_area.obj");
	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");
	CResourceManager::Load<CModelX>("Warrok", "Character\\Enemy\\warrok2\\warrok.x");

	//シーン読み込み
	CGameSceneBase::Load();

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// プレイヤー生成
	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(0.0f, 0.0f, 0.0f);

	// ウォーロックの敵①生成
	CWarrok* enemy1 = new CWarrok
	(
		{
			CVector(110.0f, 0.0f, 30.0f),
			CVector(110.0f, 0.0f, 90.0f),
			CVector(-90.0f, 0.0f, 90.0f),
			CVector(-90.0f, 0.0f, 30.0f),
			CVector(-90.0f, 0.0f, 90.0f),
			CVector(110.0f, 0.0f, 90.0f),
		}
	);
	enemy1->Scale(1.0f, 1.0f, 1.0f);
	enemy1->Position(CVector(0.0f, 0.0, 110.0f));

	// ウォーロックの敵②生成
	CWarrok* enemy2 = new CWarrok
	(
		{
			CVector(100.0f, 0.0f, 190.0f),
			CVector(100.0f, 0.0f, 260.0f),
			CVector(-40.0f, 0.0f, 260.0f),
			CVector(-40.0f, 0.0f, 190.0f),
		}
	);
	enemy2->Scale(1.0f, 1.0f, 1.0f);
	enemy2->Position(CVector(-40.0f, 0.0, 190.0f));

	// ウォーロックの敵③生成
	CWarrok* enemy3 = new CWarrok
	(
		{
			CVector(-40.0f, 0.0f, 320.0f),
			CVector(0.0f, 0.0f, 390.0f),
			CVector(0.0f, 0.0f, 450.0f),
			CVector(-170.0f, 0.0f, 450.0f),
			CVector(-170.0f, 0.0f, 320.0f),
		}
	);
	enemy3->Scale(1.0f, 1.0f, 1.0f);
	enemy3->Position(CVector(0.0f, 0.0, 100.0f));

	// スイッチを生成
	CPushSwitch* push_switch = new CPushSwitch(CVector(-93.0f, 0.0f, 163.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch->SetDebugName("Switch1");
#endif

	// スイッチを生成
	CPushSwitch* push_switch2 = new CPushSwitch(CVector(-175.0f, 0.0f, 243.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch2->SetDebugName("Switch2");
#endif

	// スイッチを生成
	CPushSwitch* push_switch3 = new CPushSwitch(CVector(-153.0f, 0.0f, 127.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch3->SetDebugName("Switch3");
#endif

	// スイッチを生成
	CPushSwitch* push_switch4 = new CPushSwitch(CVector(175.0f, 0.0f, 313.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch4->SetDebugName("Switch4");
#endif

	// スイッチを生成
	CPushSwitch* push_switch5 = new CPushSwitch(CVector(-85.0f, 0.0f, 433.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch5->SetDebugName("Switch5");
#endif

	// スイッチで開く扉①を作成
	CSwitchDoor* door = new CSwitchDoor
	(
		CVector(-185.0f, 0.0f, 145.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door->SetAnimPos
	(
		CVector(-155.0f, 0.0f, 145.0f),	// 扉を開いた時の座標
		CVector(-185.0f, 0.0f, 145.0f)	// 扉を閉じた時の座標
	);
	door->AddSwitch(push_switch2);		// 押したら開くスイッチを追加
	door->AddSwitch(push_switch3);		// 押したら開くスイッチを追加
	door->AddSwitch(push_switch5);		// 押したら開くスイッチを追加

	// スイッチで開く扉②を作成
	CSwitchDoor* door2 = new CSwitchDoor
	(
		CVector(0.0f, 0.0f, 75.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door2->SetAnimPos
	(
		CVector(-75.0f, 0.0f, 225.0f),	// 扉を開いた時の座標
		CVector(-75.0f, 0.0f, 195.0f)	// 扉を閉じた時の座標
	);
	door2->AddSwitch(push_switch2);		// 押したら開くスイッチを追加
	door2->AddSwitch(push_switch3);		// 押したら開くスイッチを追加
	door2->AddSwitch(push_switch5);		// 押したら開くスイッチを追加

	// スイッチで開く扉③を作成
	CSwitchDoor* door3 = new CSwitchDoor
	(
		CVector(45.0f, 0.0f, 345.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door3->SetAnimPos
	(
		CVector(45.0f, 0.0f, 375.0f),	// 扉を開いた時の座標
		CVector(45.0f, 0.0f, 345.0f)	// 扉を閉じた時の座標
	);
	door3->AddSwitch(push_switch4);		// 押したら開くスイッチを追加

	// スイッチで開く扉④を作成
	CSwitchDoor* door4 = new CSwitchDoor
	(
		CVector(-75.0f, 0.0f, 385.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door4->SetAnimPos
	(
		CVector(-75.0f, 0.0f, 415.0f),	// 扉を開いた時の座標
		CVector(-75.0f, 0.0f, 385.0f)	// 扉を閉じた時の座標
	);
	door4->AddSwitch(push_switch);		// 押したら開くスイッチを追加
	door4->AddSwitch(push_switch2);		// 押したら開くスイッチを追加
	door4->AddSwitch(push_switch3);		// 押したら開くスイッチを追加
	door4->AddSwitch(push_switch4);		// 押したら開くスイッチを追加
	door4->AddSwitch(push_switch5);		// 押したら開くスイッチを追加

	CClearArea* clearArea = new CClearArea
	(
		CVector(-105.0f, 0.0f, 385.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);

	// CGameCamera2のテスト
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);
}

// シーン更新処理
void CGameScene3::Update()
{
	CGameSceneBase::Update();

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
}
