#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (AnimationIndex() != 3 && AnimationIndex() != 4)
	{
		//���v���ɉ�]
		if (mInput.Key('A'))
		{
			//Y����]�l��2���Z����
			mRotation = mRotation + CVector(0.0f, 2.0f, 0.0f);
		}
		//�����v���ɉ�]
		if (mInput.Key('D'))
		{
			//Y����]�l��2���Z����
			mRotation = mRotation - CVector(0.0f, 2.0f, 0.0f);
		}

		//�������[�V�����ɐ؂�ւ��A�O���ɐi��
		if (mInput.Key('W'))
		{
			//�����A�j���[�V�����ɐ؂�ւ���
			ChangeAnimation(1, true, 60);
			//�O����0.1�i��
			mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		//�ҋ@���[�V����
		else if (!mInput.Key('W'))
		{
			//�ҋ@�A�j���[�V�����ɐ؂�ւ���
			ChangeAnimation(0, true, 60);
		}
	}
	//�U�����[�V����
	if (mInput.Key(VK_SPACE) && AnimationIndex() != 4)
	{
		//�U���A�j���[�V�����ɐ؂�ւ���
		ChangeAnimation(3, false, 30);
	}
	//�U�����[�V����2�ɐ؂�ւ���
	else if (AnimationIndex() == 3 && IsAnimationFinished() == true)
	{
		ChangeAnimation(4, false, 30);
	}
	//�ҋ@���[�V�����ɖ߂�
	else if (AnimationIndex() == 4 && IsAnimationFinished() == true)
	{
		ChangeAnimation(0, true, 60);
	}

	CXCharacter::Update();
}