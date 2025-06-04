#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CTestField;

//�^�C�g���V�[��
class CTestScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CTestScene();
	//�f�X�g���N�^
	~CTestScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenu* mpGameMenu;
	CTestField* mpField;
};
