#include "CRDoor.h"

// �R���X�g���N�^
CRDoor::CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	:CDoor(pos, angle, openPos, "RightDoor", "RightDoorCol")
{
#if _DEBUG
	SetDebugName("�E�̔�");
#endif
}

// �f�X�g���N�^
CRDoor::~CRDoor()
{
}