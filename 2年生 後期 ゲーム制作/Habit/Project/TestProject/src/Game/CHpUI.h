#pragma once
#include "CTask.h"
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CHpUI : public CTask
{
public:
	// �R���g���N�^
	CHpUI();
	// �f�X�g���N�^
	~CHpUI();

	//�\�����W��ݒ�iCVector2�Łj
	void SetPos(const CVector2& pos);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpBlueBae;
	CImage* mpRedBar;

};