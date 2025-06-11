#pragma once
#include "CBillBoardImage.h"

class CHandGlow : public CBillBoardImage
{
public:
	// �R���X�g���N�^
	CHandGlow(ETag tag);
	// �f�X�g���N�^
	~CHandGlow();

	/// <summary>
	/// �e�p�����[�^��ݒ�
	/// </summary>
	/// <param name="pos">�����ʒu</param>
	/// <param name="dir">�ړ�����</param>
	void Setup(const CVector& pos, const CVector& dir);

	// �u�����h�^�C�v��ݒ�
	void SetBlendType(EBlend type);

	// ���̃X�P�[���̍ő�l��ݒ�
	void SeGlowScale(float glowScale);
	// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎��Ԃ�ݒ�
	void SetGlowScaleAnimTime(float glowScaleAnimTime);

	// �폜�t���O�������Ă��邩�ǂ���
	bool IsDeath() const;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����f�[�^
	static TexAnimData msAnimData;
	bool mIsDeath;		// �폜�t���O
	// ���̃X�P�[���̍ő�l
	float mGlowScale;
	// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
	float mGlowScaleAnimTime;

};
