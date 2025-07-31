#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CField;

//�^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();
	//�V�[���ǂݍ���
	void Load() override;
	//�V�[���̍X�V����
	void Update() override;

private:
	CGameMenu* mpGameMenu;
	CField* mpField;
};
