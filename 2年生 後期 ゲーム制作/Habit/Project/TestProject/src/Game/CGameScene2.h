#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;

//�^�C�g���V�[��
class CGameScene2 :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene2();
	//�f�X�g���N�^
	~CGameScene2();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenu* mpGameMenu;
};
