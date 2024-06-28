#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (mInput.Key('A'))
	{
		//Y����]�l��2���Z����
		mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
	}

	if (mInput.Key('D'))
	{
		//Y����]�l��2���Z����
		mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
	}

	if (mInput.Key('W'))
	{
		//�����A�j���[�V�����ɐ؂�ւ���
		ChangeAnimation(1, true, 60);
		//�O����0.1�i��
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	else if (!mInput.Key('W'))
	{
		//�ҋ@�A�j���[�V�����ɐ؂�ւ���
		ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}