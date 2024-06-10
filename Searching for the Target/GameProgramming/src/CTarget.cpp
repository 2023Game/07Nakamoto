#include "CTarget.h"

//�ړ����x
#define VELOCITY CVector(0.1f,0.0f,0.0f)
//�ړ��͈�
#define RANGE 300

//�R���X�g���N�^
CTarget::CTarget(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale , EState state)
	: count(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.8f)
{
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�
	mType = state;
}

//�Փˏ���
void CTarget::Collision(CCollider* m, CCollider* o)
{
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE:	//���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			//�Փ˂��Ă���Ƃ��͖����ɂ���
			mEnabled = false;
			break;
		}
	}
}

//�Փˏ���
void CTarget::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mCollider.ChangePriority();
	//�Փˏ����̎��s
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

//�X�V����
void CTarget::Update()
{
	//�s����X�V
	CTransform::Update();

	switch (mType)
	{
	case CTarget::EState::ESTAY:		//�؍�
		
		break;

	case CTarget::EState::EMOVE1:	//���ړ�

		//�ʒu���ړ�
		if (count < RANGE / 2)
		{
			mPosition = mPosition + VELOCITY * mMatrixRotate;
			count++;
		}
		else if (count < RANGE * 1.5f)
		{
			mPosition = mPosition - VELOCITY * mMatrixRotate;
			count++;
		}
		else
		{
			count = -RANGE / 2;
		}
		break;
	}

	
}