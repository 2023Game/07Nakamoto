#pragma once
#include "CGameSceneBase.h"

//�^�C�g���V�[��
class CGameScene :public CGameSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
};
