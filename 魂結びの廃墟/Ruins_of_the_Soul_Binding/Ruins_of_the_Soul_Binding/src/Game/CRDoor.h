#pragma once
#include "CDoor.h"

class CRDoor : public CDoor
{
public:
	// コンストラクタ
	CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// デストラクタ
	~CRDoor();
};