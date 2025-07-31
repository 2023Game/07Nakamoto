#include "CGameOverScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CCamera.h"
#include "CGameOverUI.h"
#include "CBGMManager.h"

CGameOverScene::CGameOverScene()
	:CSceneBase(EScene::eGameOver)
{

}

CGameOverScene::~CGameOverScene()
{
}

//�V�[���ǂݍ���
void CGameOverScene::Load()
{
	// �^�C�g��BGM���Đ�
	CBGMManager::Instance()->Play(EBGMType::eTitle);

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
	// �Q�[���N���A��ʂ�
	if (mpGameOverUI->IsEnd())
	{
		// �R���e�B�j���[�Ȃ�΁A�V�[����ǂݍ���
		if (mpGameOverUI->IsContinue())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		else if(mpGameOverUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpGameOverUI->IsExitGame())
		{
			System::ExitGame();
		}
	}

}
