#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CGameClearUI;

// �Q�[���N���A�V�[��
class CGameClearScrene :public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameClearScrene();
	// �f�X�g���N�^
	~CGameClearScrene();
	//�V�[���ǂݍ���
	void Load() override;
	//�V�[���̍X�V����
	void Update() override;

private:
	CGameClearUI* mpGameClearUI;
};