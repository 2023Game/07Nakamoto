#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CGameOverUI;

// �Q�[���I�[�o�[�V�[��
class CGameOverScene :public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();
	//�V�[���ǂݍ���
	void Load() override;
	//�V�[���̍X�V����
	void Update() override;

private:
	CGameOverUI* mpGameOverUI;

};