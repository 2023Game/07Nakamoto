#include "CXPlayer.h"
#include "CActionCamera.h"

CXPlayer::CXPlayer()
	: mColShereBody(this, nullptr, CVector(), 0.5f)
	, mColShereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColShereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f,CCollider::ETag::ESWORD)
{
}

void CXPlayer::Update()
{
	if (AnimationIndex() != 3 && AnimationIndex() != 4)
	{
		//�J�����̑O��
		CVector cameraZ = CActionCamera::Instance()->VectorZ();
		//�J�����̍�����
		CVector cameraX = CActionCamera::Instance()->VectorX();
		//�L�����N�^�̑O��
		CVector charZ = mMatrixRotate.VectorZ();
		//XZ���ʂɂ��Đ��K��
		cameraZ.Y(0.0f); cameraZ = cameraZ.Normalize();
		cameraX.X(0.0f); cameraX = cameraX.Normalize();
		charZ.Y(0.0f);   charZ = charZ.Normalize();
		//�ړ������̐ݒ�
		CVector move;
		if (mInput.Key('A')) {
			move = move + cameraX;
		}
		if (mInput.Key('D')) {
			move = move - cameraX;
		}
		if (mInput.Key('W')) {
			move = move + cameraZ;
		}
		if (mInput.Key('S')) {
			move = move - cameraZ;
		}
		//�ړ�����
		if (move.Length() > 0.0f)
		{
			//�V��
			const float MARGIN = 0.06f;
			//���K��
			move = move.Normalize();
			//�����̌����ƌ��������������ŊO��
			float cross = charZ.Cross(move).Y();
			//�����̌����ƌ��������������œ���
			float dot = charZ.Dot(move);
			//�O�ς��v���X�͍���]
			if (cross > MARGIN) {
				mRotation.Y(mRotation.Y() + 5.0f);
			}
			//�O�ς��}�C�i�X�͍���]
			else if (cross < MARGIN) {
				mRotation.Y(mRotation.Y() - 5.0f);
			}
			//�O��̌����������Ƃ����ς�1.0
			else if (dot < 1.0f - MARGIN) {
				mRotation.Y(mRotation.Y() - 5.0f);
			}
			//�ړ������ֈړ�
			mPosition = mPosition + move * 0.1;
			ChangeAnimation(1, true, 60);
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

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColShereBody.Matrix(&mpCombinedMatrix[9]);
	//��
	mColShereHead.Matrix(&mpCombinedMatrix[12]);
	//��
	mColShereSword.Matrix(&mpCombinedMatrix[22]);
}