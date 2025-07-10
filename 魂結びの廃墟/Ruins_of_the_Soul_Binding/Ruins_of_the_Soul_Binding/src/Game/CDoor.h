#pragma once
#include "CDoorBase.h"
class CRoom;

class CDoor : public CDoorBase
{
public:
	// コンストラクタ
	CDoor(const CVector& pos, const CVector& angle, const CVector& openPos, std::string modelName, std::string colName);
	// デストラクタ
	virtual ~CDoor();

	// 更新処理
	void Update() override;

private:
	CVector mClosePos;	// 扉が閉まっている時の座標
	CVector mOpenPos;	// 扉が開いている時の座標
	float mAnimTime;	// 開閉時間
	float mElapsedTime;	// 経過時間保存用
};