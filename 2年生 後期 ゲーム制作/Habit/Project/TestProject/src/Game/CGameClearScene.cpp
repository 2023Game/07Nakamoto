#include "CGameClearScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CCamera.h"
#include "CGameClearUI.h"
#include "CBGMManager.h"

// �R���X�g���N�^
CGameClearScrene::CGameClearScrene()
	: CSceneBase(EScene::eClear)
{
}

// �f�X�g���N�^
CGameClearScrene::~CGameClearScrene()
{
}

//�V�[���ǂݍ���
void CGameClearScrene::Load()
{
	// TODO:�����Ȃ� BGM�̍폜
	CBGMManager::ClearInstance();

	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 50.0f, 75.0f),
		CVector::zero
	);

	mpGameClearUI = new CGameClearUI();
	AddTask(mpGameClearUI);
}

//�V�[���̍X�V����
void CGameClearScrene::Update()
{
	// �Q�[���N���A��ʂ�
	if (mpGameClearUI->IsEnd())
	{
		// �R���e�B�j���[�Ȃ�΁A�V�[����ǂݍ���
		if (mpGameClearUI->IsContinue())
		{
			CSceneManager::Instance()->LoadScene(EScene::eBootMenu);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpGameClearUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
