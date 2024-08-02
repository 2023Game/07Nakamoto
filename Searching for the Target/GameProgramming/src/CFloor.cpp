#include "CFloor.h"

//�R���X�g���N�^
CFloor::CFloor(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//�ϊ��s��̍X�V
	CTransform::Update();
	
	mCFloor.ColliderMeshSet(this, &mMatrix, CFloor::GetModelFloor() , CCollider::ETag::ESLOPE);
}

//static�ϐ��̒�`
CModel CFloor::mModelFloor;
//���̃��f�����擾����
CModel* CFloor::GetModelFloor()
{
	return &mModelFloor;
}