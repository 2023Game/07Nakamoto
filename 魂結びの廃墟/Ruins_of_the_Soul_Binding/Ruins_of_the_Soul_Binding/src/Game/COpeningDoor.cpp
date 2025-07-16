#include "COpeningDoor.h"
#include "CNavManager.h"
#include "CRoom.h"

#define DOOR_WIDTH 11.0f	// ドアの幅

#define UI_OFFSET_POS_Y	10.0f
#define UI_OFFSET_POS_Z	 -4.95f

// コンストラクタ
COpeningDoor::COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle, CRoom* room)
	: CDoorBase(pos, angle, "OpeningDoor", "OpeningDoorCol")
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
{
	mOpenAngle = openAngle;
	mCloseAngle = angle;
	Position(pos);
	Rotation(mIsOpened ? mOpenAngle : mCloseAngle);

	// UIの座標を設定
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}

// デストラクタ
COpeningDoor::~COpeningDoor()
{
}

// 更新処理
void COpeningDoor::Update()
{
	if (mIsPlaying)
	{
		CVector strtAngle = mIsOpened ? mCloseAngle : mOpenAngle;
		CVector endAngle = mIsOpened ? mOpenAngle : mCloseAngle;

		// 時間経過による開閉アニメーション
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector angle = CVector::Lerp(strtAngle, endAngle, per);
			Rotation(CQuaternion(angle));
			mElapsedTime += Times::DeltaTime();
		}
		// アニメーション時間を経過した
		else
		{
			Rotation(CQuaternion(endAngle));
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}

	// UIの座標を設定
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}
