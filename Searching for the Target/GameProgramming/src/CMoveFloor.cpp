#include "CMoveFloor.h"

//�f�t�H���g�R���X�g���N�^
CMoveFloor::CMoveFloor()
{
	
}

//�R���X�g���N�^
CMoveFloor::CMoveFloor(CVector& pos,
	 CVector& rot, CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
}