#include "CHandGlow.h"
#include "Easing.h"

// ���̃X�P�[���l�̍ő�l
#define GLOW_SCALE 10.0f
// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎���
#define GLOW_SCALE_ANIM_TIME 3.0f
// �A�j���[�V������1�R�}�\������
#define ANIM_TIME 0.0625f
// ���̃G�t�F�N�g�̃A�j���[�V�����f�[�^
TexAnimData CHandGlow::msAnimData = TexAnimData(1, 10, true, 64, ANIM_TIME);

// �R���X�g���N�^
CHandGlow::CHandGlow(ETag tag)
	:CBillBoardImage("Effect\\handglow.png",tag,ETaskPauseType::eGame)
	, mIsDeath(false)
	, mGlowScale(GLOW_SCALE)
	, mGlowScaleAnimTime(GLOW_SCALE_ANIM_TIME)
{
	SetAnimData(&msAnimData);
	SetAlpha(0.7f);
}

// �f�X�g���N�^
CHandGlow::~CHandGlow()
{
}

// �e�p�����[�^��ݒ�
void CHandGlow::Setup(const CVector& pos, const CVector& dir)
{
	Position(pos);
}

// �u�����h�^�C�v��ݒ�
void CHandGlow::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// ���̃X�P�[���̍ő�l��ݒ�
void CHandGlow::SeGlowScale(float glowScale)
{
	mGlowScale = glowScale;
}

// ���̃X�P�[���l���ő�l�ɂȂ�܂ł̎��Ԃ�ݒ�
void CHandGlow::SetGlowScaleAnimTime(float glowScaleAnimTime)
{
	mGlowScaleAnimTime = glowScaleAnimTime;
}

// �폜�t���O�������Ă��邩�ǂ���
bool CHandGlow::IsDeath() const
{
	return mIsDeath;
}

// �X�V
void CHandGlow::Update()
{
	// ���N���X�̍X�V����
	CBillBoardImage::Update();


}
