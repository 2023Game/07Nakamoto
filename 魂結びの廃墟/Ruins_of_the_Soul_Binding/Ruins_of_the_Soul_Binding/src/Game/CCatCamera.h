#pragma once
#include "CCamera.h"

class CCat;

// 猫のカメラ
class CCatCamera : public CCamera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="cat">始点となる猫のクラス</param>
	CCatCamera(CCat* cat);
	// デストラクタ
	~CCatCamera();

	// 後更新
	void LateUpdate() override;

private:
	CCat* mpCat;	// 視点となる猫のクラス
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;

};