#include "CMoveFloor.h"
#include "CCollisionManager.h"

#define MOVE CVector(0.0f, 0.0f, 0.1f)

//static�ϐ��̒�`
CModel CMoveFloor::mModelFloor;

//�������̐ݒ�
CMoveFloor::CMoveFloor(const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mMove(MOVE)
	, mTime(0)
	, mFlag(false)
	, mCLine(this, &mMatrix, CVector(0.0f, 0.0f, 5.0f), CVector(0.0f, 0.0f, -5.0f))
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
	if (mFlag)
	{
		if (mTime > 0)
		{
			mTime--;
		}
		else if (mTime == 0)
		{
			mFlag = false;
		}
		
	}
	else if (!mFlag)
	{
		if (mTime <= 0)
			mTime = 120;

		mPosition = mPosition + mMove;

		//�s��̍X�V
		CTransform::Update();
	}
}

void CMoveFloor::Collision(CCollider* m, CCollider* o)
{
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->GetType())
	{
	case CCollider::EType::ELINE: //�����R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�̍X�V
				mPosition = mPosition + adjust;
				//�ړ������𔽓]
				mMove = mMove * -1;
				mFlag = true;
				//�s��̍X�V
				CTransform::Update();
			}
		}
		break;
	}
}

void CMoveFloor::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mCLine.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::GetInstance()->Collision(&mCLine, COLLISIONRANGE);
}

//�����Ă�ʂ��擾
CVector CMoveFloor::GetMove()
{
	return mMove;
}