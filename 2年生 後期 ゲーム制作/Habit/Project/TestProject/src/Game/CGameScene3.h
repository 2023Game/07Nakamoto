#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;

//�^�C�g���V�[��
class CGameScene3 :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene3();
	//�f�X�g���N�^
	~CGameScene3();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenu* mpGameMenu;
};
