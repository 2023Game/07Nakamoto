#include "CTarget.h"
#include "CCollisionManager.h"
#include "CApplication.h"

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
	mCollider.SetTag(CCollider::ETag::ETARGET);	//�^�O
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�
	mState = state;			//��Ԃ̐ݒ�
}

//�X�V����
void CTarget::Update()
{
	//�s����X�V
	CTransform::Update();

	switch (mState)
	{
	case CTarget::EState::ESTAY:	//�؍�

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

//�Փˏ���
void CTarget::Collision(CCollider* m, CCollider* o)
{
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE:	//���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			//�^�O��nullptr�̃R���C�_�[�͔��肵�Ȃ�
			if (o->GetParent() != nullptr)
			{
				//����̃^�O���e������
				if (o->GetTag() == CCollider::ETag::EBULLET)
				{
					//�Փ˂����Ƃ��͖����ɂ���
					mEnabled = false;

					CApplication::GetUi()->AddPoint();
				}
			}
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

//static�ϐ��̒�`
CModel CTarget::mModelTarget;

//�I�̃��f�����擾����
CModel* CTarget::GetModelTarget()
{
	return &mModelTarget;
}