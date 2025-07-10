#include "CDoor.h"
#include "CNavManager.h"
#include "CRoom.h"

// コンストラクタ
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

// デストラクタ
CDoor::~CDoor()
{
}

// 更新処理
void CDoor::Update()
{
	if (mIsPlaying)
	{
		CVector startPos = mIsOpened ? mClosePos : mOpenPos;
		CVector endPos = mIsOpened ? mOpenPos : mClosePos;

		// 時間経過による開閉アニメーション
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector pos = CVector::Lerp(startPos, endPos, per);
			Position(pos);
			mElapsedTime += Times::DeltaTime();
		}
		// アニメーション時間を経過した
		else
		{
			Position(endPos);
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}
}