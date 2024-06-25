#include "CMoveFloor.h"

//static変数の定義
//CModel CMoveFloor::mModelRedCube;
//CModel CMoveFloor::mModelBuleCube;

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

//CModel* CMoveFloor::GetModelRedCube()
//{
//	return &mModelRedCube;
//

//CModel* CMoveFloor::GetModelBlueCube()
//{
//	return &mModelBuleCube;
//}

//void CMoveFloor::Update()
//{
//
//}
