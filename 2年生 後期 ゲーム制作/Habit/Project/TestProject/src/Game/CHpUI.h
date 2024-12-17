#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CHpUI : public CUIBase
{
public:
	// �R���g���N�^
	CHpUI();
	// �f�X�g���N�^
	~CHpUI();

	// �ő�l��ݒ�
	void SetMaxPoint(int point);
	// ���ݒl��ݒ�
	void SetCurPoint(int point);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// ���݂̃|�C���g���Q�[�W�ɔ��f
	void ApplyPoint();

	CImage* mpBlueBar;
	CImage* mpRedBar;
	CVector2 mBaseBarSize;

	int mMaxPoint;	// �ő�l
	int mCurPoint;	// ���ݒl
	float mPercent;	// �|�C���g�c�ʂ̊���

};