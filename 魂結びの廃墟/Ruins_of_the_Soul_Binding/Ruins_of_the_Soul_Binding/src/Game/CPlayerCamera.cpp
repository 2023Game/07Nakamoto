#include "CPlayerCamera.h"
#include "CInput.h"
#include "Maths.h"

// カメラの回転速度
#define ROTATE_SPEED 0.1f
// カメラの上下回転の範囲
#define ROTATE_RANGE_X 45.0f

// コンストラクタ
CPlayerCamera::CPlayerCamera(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
	SetPauseType(ETaskPauseType::eGame);
}

// デストラクタ
CPlayerCamera::~CPlayerCamera()
{
}

void CPlayerCamera::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CPlayerCamera::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

// 後更新
void CPlayerCamera::LateUpdate()
{
	// 追従するターゲットが設定されていれば、
	if (mFollowTargetTf != nullptr)
	{
		// マウスの移動量に合わせて、カメラの回転角度を変更
		CVector2 delta = CInput::GetDeltaMousePos();
		float x = Math::Clamp(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, -ROTATE_RANGE_X, ROTATE_RANGE_X);
		float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
		mRotateAngle.X(x);
		mRotateAngle.Y(y);

		// 回転値を求めて、注視点から視点までのベクトルを回転させることで、
		// 視点の位置を更新する
		CQuaternion rot = CQuaternion(mRotateAngle);
		mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		mTargetEye = mAt + rot * mFollowDefaultEyeVec;
		mEye = mTargetEye;
	}

#if _DEBUG
	//CDebugPrint::Print("Angle:%f, %f, %f\n", mRotateAngle.X(), mRotateAngle.Y(), mRotateAngle.Z());
#endif

	// 設定されているコライダーと衝突する場合は、
	// カメラの位置を押し出す
	ApplyCollision();

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);
}
