#include "CMoveFloor.h"

//static�ϐ��̒�`
//CModel CMoveFloor::mModelRedCube;
//CModel CMoveFloor::mModelBuleCube;

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
