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

	/// <summary>
	/// 追従するターゲットを設定
	/// </summary>
	/// <param name="target">追従するターゲット</param>
	void SetFollowTargetTf(CTransform* target) override;

	/// <summary>
	/// 注視する位置を設定（視点 + 注視点 + 上ベクトル）
	/// </summary>
	/// <param name="eye">カメラの位置</param>
	/// <param name="at">注視する位置</param>
	/// <param name="up">上ベクトル</param>
	/// <param name="updateTargetEye">視点の目標位置も更新するかどうか</param>
	void LookAt(const CVector& eye, const CVector& at,
		const CVector& up, bool updateTargetEye = true) override;

	// 更新
	void Update() override;

private:
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;
};