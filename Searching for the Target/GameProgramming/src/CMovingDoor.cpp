#include "CMovingDoor.h"

//static�ϐ��̒�`
CModel CMovingDoor::mModelCube;

CMovingDoor::CMovingDoor()
	: mMove(0)
	, mMoveDistance(0)
	, mpSwhitch(nullptr)
{

}

//�R���X�g���N�^
CMovingDoor::CMovingDoor(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(0)
	, mMoveDistance(0)
{
	mpSwhitch = parent;
	mpModel = &mModelCube;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 5;

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

void CMovingDoor::SetMovingDoor(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	mpSwhitch = parent;
	mpModel = &mModelCube;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 10;

	//�s��̍X�V
	CTransform::Update();

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//�X�V����
void CMovingDoor::Update()
{
	//�e��Swhich�I�u�W�F�N�g�ɓ���������
	if (mpSwhitch->GetFlag() == true && mMove <= mMoveDistance) 
	{
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f);
		mMove += 0.1;
	}

	//�s��̍X�V
	CTransform::Update();
}

//���f���̎擾
CModel* CMovingDoor::GetModelCube()
{
	return &mModelCube;
}