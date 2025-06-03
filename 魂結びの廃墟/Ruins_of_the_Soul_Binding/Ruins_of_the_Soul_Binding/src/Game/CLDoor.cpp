#include "CLDoor.h"

// コンストラクタ
CLDoor::CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	:CDoor(pos, angle, openPos, "LeftDoor", "LeftDoorCol")
{
#if _DEBUG
	SetDebugName("左の扉");
#endif
}

// デストラクタ
CLDoor::~CLDoor()
{
}