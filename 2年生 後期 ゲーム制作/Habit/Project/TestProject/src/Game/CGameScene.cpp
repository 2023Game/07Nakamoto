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
#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "CSwitchDoor.h"

#include "CTouch.h"
#include "CPlayer3.h"

#include "ItemData.h"
#include "CChoco.h"

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpInventory(nullptr)
{
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
	// ���ɓǂݍ��܂��V�[�����Q�[���V�[���łȂ���΁A
	// �A�C�e���̃��\�[�X��S�Ĕj������
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
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
	CResourceManager::Load<CModel>("Shield", "Weapon\\Shield\\shield.obj");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\mutant\\mutant.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");
	CResourceManager::Load<CModel>("Switch", "Object\\Switch.obj");

	CResourceManager::Load<CModel>("Spider_Web", "Character\\Enemy\\trap\\spider_web.obj");

	CResourceManager::Load<CModel>("Touch", "Object\\Touch.obj");
	CResourceManager::Load<CModel>("Obj", "Object\\mm.obj");

	CResourceManager::Load<CModelX>("Player3", "Character\\Player3\\Acquire.x");
	CResourceManager::Load<CModelX>("Ghost", "Character\\Enemy\\ghost\\ghost.x");

	// �A�C�e���̃��\�[�X��S�ǂݍ���
	Item::LoadItemResources();

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();
	// �t�B�[���h�쐬
	new CField();

	// �T�{�e���̓G���쐬
	CCactus* cactus = new CCactus();
	cactus->Position(0.0f, 0.0f, -100.0f);

	//CPlayer* player = new CPlayer();
	//player->Scale(1.0f, 1.0f, 1.0f);
	//player->Position(-50.0f, 1.0f, 0.0f);

	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(-50.0f, 0.0f, 10.0f);

	CPlayer3* player3 = new CPlayer3();
	player3->Scale(1.0f, 1.0f, 1.0f);
	player3->Position(-50.0f, 0.0f, -10.0f);

	// �G�@����
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
	// �G�A����
	CEnemy2* enemy2 = new CEnemy2
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

	// ����(��)�̕`��̊m�F	�폜�\��
	CTouch* touch = new CTouch(CVector(0.0f, 0.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f), CVector(0.3f, 0.3f, 0.3f));

	// �`���R�𐶐�
	CChoco* choco = new CChoco();
	choco->Position(0.0f, 10.0f, 20.0f);

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch = new CPushSwitch(CVector(-30.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f,1.0f,1.0f));
#if _DEBUG
	push_switch->SetDebugName("Switch1");
#endif

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch2 = new CPushSwitch(CVector(-50.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch2->SetDebugName("Switch2");
#endif

	// �X�C�b�`�ŊJ�����@���쐬
	CSwitchDoor* door = new CSwitchDoor
	(
		CVector(-50.0f, 0.0f, -50.0f),
		CVector( 0.0f,  0.0f,   0.0f),
		CVector( 2.0f,  5.0f,   2.0f)
	);
	door->SetAnimPos
	(
		CVector(-30.0f, 0.0f, -50.0f),	// �����J�������̍��W
		CVector(-50.0f, 0.0f, -50.0f)	// ����������̍��W
	);
	door->AddSwitch(push_switch);		// ��������J���X�C�b�`��ǉ�

	// �X�C�b�`�ŊJ�����A���쐬
	CSwitchDoor* door2 = new CSwitchDoor
	(
		CVector(-120.0f, 0.0f, -50.0f),
		CVector(   0.0f, 0.0f,   0.0f),
		CVector(   2.0f, 5.0f,   2.0f)
	);
	door2->SetAnimPos
	(
		CVector(-100.0f, 0.0f, -50.0f),	// �����J�������̍��W
		CVector(-120.0f, 0.0f, -50.0f)	// ����������̍��W
	);
	door2->AddSwitch(push_switch);		// ��������J���X�C�b�`��ǉ�
	door2->AddSwitch(push_switch2);		// ��������J���X�C�b�`��ǉ�

	// CGameCamera�̃e�X�g
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2�̃e�X�g
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);

	// �Q�[�����j���[���쐬
	mpInventory = new CInventory();
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
		CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	if (!mpInventory->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpInventory->Open();
		}
	}
	else
	{
		if (CInput::PushKey('M'))
		{
			mpInventory->Close();
		}
	}
}
