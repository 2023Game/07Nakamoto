#include "CGameOverScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CCamera.h"
#include "CGameOverUI.h"

//�R���X�g���N�^
CGameOverScene::CGameOverScene()
	: CSceneBase(EScene::eGameOver)
{
}

//�f�X�g���N�^
CGameOverScene::~CGameOverScene()
{
}

//�V�[���ǂݍ���
void CGameOverScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpGameOverUI = new CGameOverUI();
	AddTask(mpGameOverUI);
}

//�V�[���̍X�V����
void CGameOverScene::Update()
{
	// �Q�[���I�[�o�[��ʂ�
	if (mpGameOverUI->IsEnd())
	{
		// �R���e�B�j���[�Ȃ�΁A�V�[����ǂݍ���
		if (mpGameOverUI->IsContinue())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpGameOverUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
