#pragma once
#include "CTask.h"

class CImage;

class CGameUI : public CTask
{
public:
	// �R���g���N�^
	CGameUI();
	// �f�X�g���N�^
	~CGameUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
};