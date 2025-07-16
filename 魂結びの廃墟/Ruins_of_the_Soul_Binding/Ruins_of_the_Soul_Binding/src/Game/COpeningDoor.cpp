#include "COpeningDoor.h"
#include "CNavManager.h"
#include "CRoom.h"

#define DOOR_WIDTH 11.0f	// �h�A�̕�

#define UI_OFFSET_POS_Y	10.0f
#define UI_OFFSET_POS_Z	 -4.95f

// �R���X�g���N�^
COpeningDoor::COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle, CRoom* room)
	: CDoorBase(pos, angle, "OpeningDoor", "OpeningDoorCol")
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
{
	mOpenAngle = openAngle;
	mCloseAngle = angle;
	Position(pos);
	Rotation(mIsOpened ? mOpenAngle : mCloseAngle);

	// UI�̍��W��ݒ�
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}

// �f�X�g���N�^
COpeningDoor::~COpeningDoor()
{
}

// �X�V����
void COpeningDoor::Update()
{
	if (mIsPlaying)
	{
		CVector strtAngle = mIsOpened ? mCloseAngle : mOpenAngle;
		CVector endAngle = mIsOpened ? mOpenAngle : mCloseAngle;

		// ���Ԍo�߂ɂ��J�A�j���[�V����
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector angle = CVector::Lerp(strtAngle, endAngle, per);
			Rotation(CQuaternion(angle));
			mElapsedTime += Times::DeltaTime();
		}
		// �A�j���[�V�������Ԃ��o�߂���
		else
		{
			Rotation(CQuaternion(endAngle));
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}

	// UI�̍��W��ݒ�
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}
