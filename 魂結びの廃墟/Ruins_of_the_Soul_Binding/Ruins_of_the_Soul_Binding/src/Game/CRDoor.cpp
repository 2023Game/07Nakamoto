#include "CRDoor.h"

// コンストラクタ
CRDoor::CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	:CDoor(pos, angle, openPos, "RightDoor", "RightDoorCol")
{
#if _DEBUG
	SetDebugName("右の扉");
#endif
}

// デストラクタ
CRDoor::~CRDoor()
{
}