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
	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk\\idle.x");
	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");
	// �G
	CResourceManager::Load<CModelX>(	"Warrok",			"Character\\Enemy\\Warrok\\warrok.x");
	CResourceManager::Load<CModelX>(	"Spider",			"Character\\Enemy\\Spider\\spider.x");
	// ��
	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	// ��
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall2.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col2.obj");
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

	// �e�X�g�p�̈ړ��G���A�𐧌������
	CResourceManager::Load<CModel>(		"TestWall",			"Field\\Test\\test_wall0.obj");
	CResourceManager::Load<CModel>(		"TestWallBrack",	"Field\\Test\\Brack_Wall.obj");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// ���ׂ�I�u�W�F�N�g�̊Ǘ��N���X�̍쐬
	new CInteractObjectManager();

	// �v���C���[�̊Ǘ��N���X�̍쐬
	new CPlayerManager();

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();

	// �t�B�[���h�̐���
	mpField = new CField();

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
			CVector( 90.0f, 0.0, 60.0f),
			CVector(200.0f, 0.0, 60.0f),
			CVector(200.0f, 0.0, 180.0f),
			CVector( 90.0f, 0.0, 180.0f),
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
}
