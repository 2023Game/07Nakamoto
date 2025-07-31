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

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
	// ���ׂ�I�u�W�F�N�g�̊Ǘ��N���X���폜
	CInteractObjectManager::CIearInstance();

	// ���ɓǂݍ��܂��V�[�����Q�[���V�[���łȂ���΁A
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr)
	{
		// �A�C�e���̃��\�[�X��S�Ĕj������
		Item::DeleteItemResources();

		// �Q�[��UI��j������
		CGameUI::ClearInstance();
	}
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	// �v���C���[
	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk2\\idle.x");
	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");
	// �G
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");
	CResourceManager::Load<CModelX>(	"Spider",			"Character\\Enemy\\Spider\\spider.x");
	// ��
	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	// ��
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall3.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col3.obj");
	// ��
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		"Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			"Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		"Door\\left_door_col.obj");
	CResourceManager::Load<CModel>(		"OpeningDoor",		"Door\\opening_door.obj");
	CResourceManager::Load<CModel>(		"OpeningDoorCol",	"Door\\opening_door_col.obj");
	// �d�͂̌�
	CResourceManager::Load<CModel>(		"DemonPower",		"Field\\Object\\Demon_Power\\source_of_demon_power.obj");
	// �X�C�b�`
	CResourceManager::Load<CModel>(		"Stand",			"Field\\Object\\Switch\\stand.obj");
	CResourceManager::Load<CModel>(		"Button",			"Field\\Object\\Switch\\buttun.obj");
	CResourceManager::Load<CModel>(		"StandWallCol",		"Field\\Object\\Switch\\stand_wall_col.obj");
	CResourceManager::Load<CModel>(		"StandFloorCol",	"Field\\Object\\Switch\\stand_floor_col.obj");
	// �΋�
	CResourceManager::Load<CModel>(		"Fireball",			"Weapon\\fire.obj");
	

	// �e�X�g�p�̈ړ��G���A�𐧌������
	CResourceManager::Load<CModel>(		"TestObj",			"Field\\Test\\obj4.obj");
	CResourceManager::Load<CModel>(		"TestObjFloor",		"Field\\Test\\obj4_floor.obj");
	CResourceManager::Load<CModel>(		"TestObjWall",		"Field\\Test\\obj4_wall.obj");

	CResourceManager::Load<CModel>(		"TestWall",			"Field\\Test\\test_wall0.obj");
	CResourceManager::Load<CModel>(		"TestWallBrack",	"Field\\Test\\Brack_Wall.obj");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �����Ǘ��N���X�𐶐�
	new CRoomManager();

	// ���ׂ�I�u�W�F�N�g�̊Ǘ��N���X�̐���
	new CInteractObjectManager();

	// �v���C���[�̊Ǘ��N���X�̐���
	new CPlayerManager();

	// �o�H�T���Ǘ��N���X�𐶐�
	new CNavManager();

	// �t�B�[���h�̐���
	mpField = new CField();

	// �Q�[�����j���[�𐶐�
	new CInventory();

	// UI�̐���
	new CGameUI();

	// �v���C���[�̐���
	CPlayer2* player2 = new CPlayer2();
	player2->Scale(1.0f, 1.0f, 1.0f);

	// �L�̐���
	CCat* cat = new CCat();
	cat->Scale(1.0f, 1.0f, 1.0f);
	cat->Position(10.0f, 0.0f, 0.0f);

	// �E�H�[���b�N�̐���
	CBoss* warrok = new CBoss
	(
		{
			// ���[�g1
			{
				CVector(200.0f, 0.0, 60.0f),
				CVector(200.0f, 0.0, 180.0f),
				CVector(120.0f, 0.0, 180.0f),
				CVector(120.0f, 0.0, 450.0f),
				CVector(-20.0f, 0.0, 450.0f),
				CVector(-20.0f, 0.0, 395.0f),
			},
			// ���[�g2
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
			// ���[�g3
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
			// ���[�g4
			{
				CVector(-11.5f, 0.0, -180.0f),
				CVector(-155.0f, 0.0, -180.0f),
				CVector(65.0f, 0.0, -60.0f),
				CVector(65.0f, 0.0, 60.0f),
				CVector(-90.0f, 0.0, 60.0f),
				CVector(-90.0f, 0.0, 310.0f),
				CVector(-80.0f, 0.0, 330.0f),
			},
			// ���[�g5
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

	//// �w�
	//CSpider* spider = new CSpider
	//(
	//	{
	//		CVector(100.0f,0.0f,50.0f),
	//		CVector(100.0f,0.0f, 50.0f),
	//	}
	//);
	//spider->Scale(10.0f, 10.0f, 10.0f);
	//spider->Position(100.0f, 0.0f, 50.0f);

	//// �E�H�[���b�N�̐���
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

	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CPlayerCamera�̃e�X�g
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

	// �L���_�̃J�������쐬
	CCatCamera* catCamera = new CCatCamera(cat);
	cat->SetCamera(catCamera);

	// �Q�[�����j���[���쐬
	mpGameMenu = new CGameMenu();
}

//�V�[���̍X�V����
void CGameScene::Update()
{
	// BGM�Đ����łȂ���΁ABGM���Đ�
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
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

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}
#endif
}
