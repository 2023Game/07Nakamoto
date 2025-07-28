#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CEquipmentUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CEquipmentUI();
	// �f�X�g���N�^
	~CEquipmentUI();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:

	CImage* mpEquipmentSlot;	// �����X���b�g�g�̃C���[�W
};