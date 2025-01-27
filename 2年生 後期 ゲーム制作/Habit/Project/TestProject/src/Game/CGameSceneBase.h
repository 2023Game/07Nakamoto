#pragma once
#include "CSceneBase.h"

class CInventory;

// �Q�[���V�[���̃x�[�X�N���X
class CGameSceneBase : public CSceneBase
{
public:
	// �R���X�g���N�^
	CGameSceneBase(EScene scene);
	// �f�X�g���N�^
	~CGameSceneBase();
	//�V�[���ǂݍ���
	void Load();
	// �X�V����
	void Update();

private:
	CInventory* mpInventory;

};