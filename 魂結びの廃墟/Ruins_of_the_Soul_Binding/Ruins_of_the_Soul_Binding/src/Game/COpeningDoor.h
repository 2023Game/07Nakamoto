#pragma once
#include "CDoorBase.h"
class CRoom;

class COpeningDoor : public CDoorBase
{
public:
	// コンストラクタ
	COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle, CRoom* room);
	// デストラクタ
	~COpeningDoor();

	// 更新処理
	void Update() override;

private:
	CVector mCloseAngle;	// 扉が閉まっている時の座標
	CVector mOpenAngle;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
};