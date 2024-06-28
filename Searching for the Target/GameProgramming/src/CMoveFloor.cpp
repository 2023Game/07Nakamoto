#include "CMoveFloor.h"

//デフォルトコンストラクタ
CMoveFloor::CMoveFloor()
{
	
}

//コンストラクタ
CMoveFloor::CMoveFloor(CVector& pos,
	 CVector& rot, CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
}