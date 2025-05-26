#pragma once
#include "CCamera.h"

// ゲーム中のカメラ
class CPlayerCamera : public CCamera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="eye">カメラの視点</param>
	/// <param name="center">カメラの注視点</param>
	/// <param name="isMainCamera">メインカメラかどうか</param>
	CPlayerCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// デストラクタ
	~CPlayerCamera();

	void SetFollowTargetTf(CTransform* target) override;
	void LookAt(const CVector& eye, const CVector& at,
		const CVector& up, bool updateTargetEye = true) override;

	// 更新
	void Update() override;

private:
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;
};