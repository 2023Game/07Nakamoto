#pragma once
#include "CDoor.h"

class CRDoor : public CDoor
{
public:
	// �R���X�g���N�^
	CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos);
	// �f�X�g���N�^
	~CRDoor();
};