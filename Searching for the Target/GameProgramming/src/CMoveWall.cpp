#include "CMoveWall.h"
#include "CCollisionManager.h"

//static�ϐ��̒�`
CModel CMoveWall::mModelWall;

CMoveWall::CMoveWall(CSwitch* parent, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(0)
	, mMoveDistance(0)
	, mpSwhitch(nullptr)
{
	mpSwhitch = parent;
	mpModel = &mModelWall;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mMoveDistance = 10;

	//�s��̍X�V
	CTransform::Update();

	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//�X�V����
void CMoveWall::Update()
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

//�����ǂ̃��f���̎擾
CModel* CMoveWall::GetModelWall()
{
	return &mModelWall;
}