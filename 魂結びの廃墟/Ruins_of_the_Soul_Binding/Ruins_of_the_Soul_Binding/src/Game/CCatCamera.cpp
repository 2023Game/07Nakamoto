#include "CCatCamera.h"
#include "CInput.h"
#include "Maths.h"
#include "CCat.h"

// �J�����̉�]���x
#define ROTATE_SPEED 0.1f
// �J�����̏㉺��]�͈̔�
#define ROTATE_RANGE_X 45.0f

// �R���X�g���N�^
CCatCamera::CCatCamera(CCat* cat)
	: CCamera(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 1.0f), false)
	, mpCat(cat)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// �f�X�g���N�^
CCatCamera::~CCatCamera()
{
}

// �X�V����
void CCatCamera::Update()
{
	CVector2 delta = CInput::GetDeltaMousePos();
	float x = Math::Repeat(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, 360.0f);
	float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
	mRotateAngle.X(x);
	mRotateAngle.Y(y);

	// �L�̎��_�̍��W
	mEye =	mpCat->Position() +	// �L�̍��W +
			CVector(0.0f, 7.5f, 0.0f) +	// ������̃I�t�Z�b�g�n +
			mpCat->VectorZ() * 5.0f;	// �L�̐��ʕ����ւ̃I�t�Z�b�g�l
	// �L���_�̒����_
	mAt = mEye + mpCat->VectorZ();		// �L���_�̍��W + �L�̐��ʕ����x�N�g��

	mTargetEye = mEye;

	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);
}
