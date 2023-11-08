#include "CPlayer.h"
#include "CApplication.h"
//#include "CGame.h"

#define TEXCOORD 168, 188, 158, 128	//�e�N�X�`���}�b�s���O
#define GRAVITY (TIPSIZE / 20.0f)	//�d�͉����x
#define JUMPV0 (TIPSIZE / 1.4f)		//�W�����v�̏���

//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //��]���x
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //��]���x
#define VELOCITY CVector(0.0f,0.0f,0.1f) //�ړ����x

//CPlayer(�ʒu,��],�X�P�[��)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //�s����X�V
}

//�X�V����
void CPlayer::Update()
{
	//�X�y�[�X�L�[���͂Œe����
	if (mInput.Key(VK_SPACE)) {
		bullet.Set(0.1f, 1.5f);
		bullet.Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet.Rotation(mRotation);
	}
	//S�L�[���͂ŏ����
	if (mInput.Key('S'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation - ROTATION_XV;
	}
	//W�L�[���͂ŏ����
	if (mInput.Key('W'))
	{
		//X���̉�]�l�����Z
		mRotation = mRotation + ROTATION_XV;
	}
	//D�L�[���͂ŉ�]
	if (mInput.Key('D'))
	{
		//Y���̉�]�n������
		mRotation = mRotation - ROTATION_YV;
	}
	//A�L�[���͂Ŕ����v��]
	if (mInput.Key('A'))
	{
		//Y���̉�]�n�𑝉�
		mRotation = mRotation + ROTATION_YV;
	}
	//��L�[���͂őO�i
	if (mInput.Key(VK_UP))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	//�ϊ��s��̍X�V
	CTransform::Update();
}