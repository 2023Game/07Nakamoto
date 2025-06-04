#include "CCatCamera.h"
#include "CInput.h"
#include "Maths.h"
#include "CCat.h"

// カメラの回転速度
#define ROTATE_SPEED 0.1f
// カメラの上下回転の範囲
#define ROTATE_RANGE_X 45.0f

// コンストラクタ
CCatCamera::CCatCamera(CCat* cat)
	: CCamera(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 1.0f), false)
	, mpCat(cat)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// デストラクタ
CCatCamera::~CCatCamera()
{
}

// 更新処理
void CCatCamera::Update()
{
	CVector2 delta = CInput::GetDeltaMousePos();
	float x = Math::Repeat(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, 360.0f);
	float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
	mRotateAngle.X(x);
	mRotateAngle.Y(y);

	// 猫の視点の座標
	mEye =	mpCat->Position() +	// 猫の座標 +
			CVector(0.0f, 7.5f, 0.0f) +	// 上方向のオフセット地 +
			mpCat->VectorZ() * 5.0f;	// 猫の正面方向へのオフセット値
	// 猫視点の注視点
	mAt = mEye + mpCat->VectorZ();		// 猫視点の座標 + 猫の正面方向ベクトル

	mTargetEye = mEye;

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);
}
