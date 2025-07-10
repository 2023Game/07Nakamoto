#include "CDoor.h"
#include "CNavManager.h"
#include "CRoom.h"

// �R���X�g���N�^
CDoor::CDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CDoorBase(pos, angle, modelName, colName)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
{
	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);
	Rotation(angle);
}

// �f�X�g���N�^
CDoor::~CDoor()
{
}

// �X�V����
void CDoor::Update()
{
	if (mIsPlaying)
	{
		CVector startPos = mIsOpened ? mClosePos : mOpenPos;
		CVector endPos = mIsOpened ? mOpenPos : mClosePos;

		// ���Ԍo�߂ɂ��J�A�j���[�V����
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector pos = CVector::Lerp(startPos, endPos, per);
			Position(pos);
			mElapsedTime += Times::DeltaTime();
		}
		// �A�j���[�V�������Ԃ��o�߂���
		else
		{
			Position(endPos);
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}
}