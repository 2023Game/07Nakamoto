#pragma once
#include "CDoor.h"
class CLDoor : public CDoor
{
public:
	// �R���X�g���N�^
	CLDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// �f�X�g���N�^
	~CLDoor();
};