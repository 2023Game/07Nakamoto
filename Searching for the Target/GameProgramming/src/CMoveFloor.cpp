#include "CMoveFloor.h"
#include "CCollisionManager.h"

//static�ϐ��̒�`
CModel CMoveFloor::mModelFloor;

//�f�t�H���g�R���X�g���N�^
CMoveFloor::CMoveFloor()
	: mMove(0)
	, mMoveDistance(0)
	, mCLine(this,&mMatrix, CVector(0.0f, 0.0f, 7.0f), CVector(0.0f, 0.0f, -7.0f))
{
	
}

//�������̐ݒ�
void CMoveFloor::SetMoveFloor(const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	mpModel = &mModelFloor;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//�s��̍X�V
	CTransform::Update();
	//�R���C�_�[�̐���
	mColliderMesh.ColliderMeshSet(this, &mMatrix, mpModel);
}

//�������̃��f���̎擾
CModel* CMoveFloor::GetModelFloor()
{
	return &mModelFloor;
}

void CMoveFloor::Update()
{
	mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f);

	//�s��̍X�V
	CTransform::Update();
}

void CMoveFloor::Collision(CCollider* m, CCollider* o)
{
	
}

void CMoveFloor::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mCLine.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::GetInstance()->Collision(&mCLine, COLLISIONRANGE);
}