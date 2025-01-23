#include "CGameScene2.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer2.h"
#include "CEnemy1.h"
#include "CEnemy2.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "ItemData.h"
#include "CInventory.h"

//�R���X�g���N�^
CGameScene2::CGameScene2()
	: CSceneBase(EScene::eGame2)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CGameScene2::~CGameScene2()
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
void CGameScene2::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	CResourceManager::Load<CModel>("Map_mini", "Map\\map_mini1.obj");
	CResourceManager::Load<CModel>("Map_mini_floor", "Map\\map_mini_floor.obj");
	CResourceManager::Load<CModel>("Map_mini_Col", "Map\\map_mini1_col.obj");
	CResourceManager::Load<CModel>("Map_mini_door", "Map\\map_mini1_door.obj");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\mutant\\mutant.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");

	CResourceManager::Load<CModel>("Spider_Web", "Character\\Enemy\\trap\\spider_web.obj");

	// �A�C�e���̃��\�[�X��S�ǂݍ���
	Item::LoadItemResources();

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();
	// �t�B�[���h�쐬
	new CField();

	// �v���C���[����
	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(10.0f, 10.0f, 10.0f);

	// �G�@����
	CEnemy1* enemy = new CEnemy1
	(
		{
			CVector(70.0f, 1.0, 10.0f),
			CVector(180.0f, 1.0, 10.0f),
			CVector(180.0f, 1.0, 100.0f),
			CVector(80.0f, 1.0, 100.0f),
		}
	);
	enemy->Scale(1.0f, 1.0f, 1.0f);
	enemy->Position(CVector(70.0f, 1.0, 10.0f));
	// �G�A����
	CEnemy2* enemy2 = new CEnemy2
	(
		{
			CVector(195.0f, 0.0f, 115.0f),
			CVector(265.0f, 0.0f, 115.0f),
			CVector(265.0f, 0.0f, 180.0f),
			CVector(195.0f, 0.0f, 180.0f),
		}
	);
	enemy2->Scale(1.0f, 1.0f, 1.0f);
	enemy2->Position(CVector(195.0f, 0.0f, 115.0f));

	// CGameCamera2�̃e�X�g
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);

	// �Q�[�����j���[���쐬
	new CGameMenu();

}

//�V�[���̍X�V����
void CGameScene2::Update()
{
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	CInventory* inv = CInventory::Instance();
	if (!inv->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			inv->Open();
		}
	}

}
