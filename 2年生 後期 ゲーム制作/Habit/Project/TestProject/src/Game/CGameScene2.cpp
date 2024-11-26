#include "CGameScene2.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer2.h"
#include "CEnemy.h"
#include "CEnemy2.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"

//�R���X�g���N�^
CGameScene2::CGameScene2()
	: CSceneBase(EScene::eGame2)
	, mpGameMenu(nullptr)
{
}

//�f�X�g���N�^
CGameScene2::~CGameScene2()
{
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
	CResourceManager::Load<CModel>("Map_mini", "Map\\map_mini.obj");
	CResourceManager::Load<CModel>("Map_mini_floor", "Map\\map_mini_floor.obj");
	CResourceManager::Load<CModel>("Map_mini_Col", "Map\\map_mini_col.obj");

	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\mutant\\mutant.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");

	CResourceManager::Load<CTexture>("spider_web", "weapon\\spider_web.png");

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
	CEnemy* enemy = new CEnemy
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
	mpGameMenu = new CGameMenu();

}

//�V�[���̍X�V����
void CGameScene2::Update()
{
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
