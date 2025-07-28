#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;
class CIcon;

class CGaugeUI : public CUIBase
{
public:
	// �R���g���N�^
	CGaugeUI();
	// �f�X�g���N�^
	virtual ~CGaugeUI();

	// �ő�l��ݒ�
	void SetMaxPoint(int point);
	// ���ݒl��ݒ�
	void SetCurPoint(int point);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
	// ���݂̃|�C���g���Q�[�W�ɔ��f
	void ApplyPoint();

	// �o�[�̐F(��)
	virtual CColor CalcBarColor() const;

	CImage* mpWhiteImag;	// �Q�[�W�̃C���[�W
	CImage* mpGaugeImag;	// ���C���[�W
	CImage* mpFrameImage;	// �Q�[�W�̘g�̃C���[�W

	CVector2 mBaseBarSize;	// �Q�[�W�̃C���[�W�T�C�Y
	int mMaxPoint;	// �ő�l
	int mCurPoint;	// ���ݒl
	float mPercent;	// �|�C���g�c�ʂ̊���

};