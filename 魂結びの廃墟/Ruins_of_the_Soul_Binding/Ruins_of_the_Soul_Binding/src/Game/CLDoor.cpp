#include "CLDoor.h"

// �R���X�g���N�^
CLDoor::CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	:CDoor(pos, angle, openPos, "LeftDoor", "LeftDoorCol")
{
#if _DEBUG
	SetDebugName("���̔�");
#endif
}

// �f�X�g���N�^
CLDoor::~CLDoor()
{
}