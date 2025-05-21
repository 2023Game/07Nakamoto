#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCactus.h"

#include "CPlayer2.h"
#include "CCat.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
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

	//CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\kyoko.x");
	CResourceManager::Load<CModelX>(	"Player2",			"Character\\Player2\\Rusk\\idle.x");

	CResourceManager::Load<CModelX>(	"Cat",				"Character\\Cat\\cat.x");

	CResourceManager::Load<CModel>(		"Floor",			"Field\\Abandoned_School_Floor.obj");
	CResourceManager::Load<CModel>(		"Wall",				"Field\\Abandoned_School_Wall.obj"); 
	CResourceManager::Load<CModel>(		"WallCol",			"Field\\Abandoned_School_Wall_Col.obj");
	CResourceManager::Load<CModel>(		"RightDoor",		"Door\\right_door.obj");
	CResourceManager::Load<CModel>(		"RightDoorCol",		"Door\\right_door_col.obj");
	CResourceManager::Load<CModel>(		"LeftDoor",			"Door\\left_door.obj");
	CResourceManager::Load<CModel>(		"LeftDoorCol",		"Door\\left_door_col.obj");

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �t�B�[���h�̐���
	mpField = new CField();

	// �T�{�e���̓G���쐬
	CCactus* cactus = new CCactus();
	cactus->Position(0.0f, 0.0f, -100.0f);

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);

	CPlayer2* player2 = new CPlayer2();
	player2->Scale(1.0f, 1.0f, 1.0f);

	CCat* cat = new CCat();
	cat->Scale(1.0f, 1.0f, 1.0f);
	cat->Position(10.0f, 0.0f, 0.0f);

	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2�̃e�X�g
	CVector atPos = player2->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(player2);
	mainCamera->AddCollider(mpField->GetFloorCol());
	mainCamera->AddCollider(mpField->GetWallCol());

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
