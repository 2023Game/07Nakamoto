#pragma once
#include "CDoor.h"
class CLDoor : public CDoor
{
public:
	// コンストラクタ
	CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// デストラクタ
	~CLDoor();
};