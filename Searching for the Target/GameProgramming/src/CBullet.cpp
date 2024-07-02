#include "CBullet.h"
#include "CCollisionManager.h"
#include <stdio.h>

//static�ϐ��̒�`
CModel CBullet::mModelBullet;

//�f�t�H���g�R���X�g���N�^
CBullet::CBullet()
	: mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.3f), 0.5f)
{

}

//�Փˏ���
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE:	//���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o)) 
		{
			if (o->GetParent()->GetTag() == CCharacter::ETag::ETARGET)
			{
				//�Փ˂��Ă���Ƃ��͖����ɂ���
				mEnabled = false;

				printf("Target��Hit\n");
			}
			else if (o->GetParent()->GetTag() == CCharacter::ETag::ESWITCH)
			{
				//�Փ˂��Ă���Ƃ��͖����ɂ���
				mEnabled = false;

				printf("Switch��Hit\n");
			}
		}
		break;
	case CCollider::EType::ETRIANGLE:	//�O�p�R���C�_�̎�
		CVector adjust;	//�����l
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			if (o->GetParent() != nullptr)
			{
				if (o->GetParent()->GetTag() == CCharacter::ETag::ESWITCH)
				{
					printf("SWitch�^�C�v");
				}
			}
			
			//�Փ˂��Ă���Ƃ��͖����ɂ���
			mEnabled = false;

			printf("�O�p�R���C�_��hit\n");
		}
		break;
	}
}

//�X�V
void CBullet::Update()
{
	//�������Ԃ̔���
	if (mLife-- > 0) 
	{
		CTransform::Update();
		//�ʒu�X�V
		mPosition = mPosition + CVector(0.0f, 0.0f, 2.0f) * mMatrixRotate;
	}
	else
	{
		//�����ɂ���
		mEnabled = false;
	}
}

//�Փˏ���
void CBullet::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mCollider.ChangePriority();
	//�Փˏ����̎��s
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

//�e�̃��f�����擾����
CModel* CBullet::GetModelBullet()
{
	return &mModelBullet;
}
