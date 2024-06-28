#include "CSwitch.h"
#include "CCollisionManager.h"
#include "CMovingDoor.h"

//�f�t�H���g�R���X�g���N�^
CSwitch::CSwitch()
	: mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mFlag(false)
{
	
}

//�X�C�b�`�̐���
void CSwitch::SetSwitch(CModel* model, const CVector& pos,
	const CVector& rot, const CVector& scale)
{
	mTag = ETag::ESWITCH;	//�^�O
	mpModel = model;		//���f��
	mPosition = pos;		//�ʒu
	mRotation = rot;		//��]
	mScale = scale;			//�g�k

}

//�X�C�b�`�ƕǂ̐ݒ�
void CSwitch::SetSwitch(CSwitch* parent, CModel* model,
	const CVector& spos, const CVector& srot,const CVector& sscale,
	const CVector& dpos, const CVector& drot, const CVector& dscale)
{
	mTag = ETag::ESWITCH;	//�^�O
	mpModel = model;		//���f��
	mPosition = spos;		//�ʒu
	mRotation = srot;		//��]
	mScale = sscale;		//�g�k

	//���̐���
	new CMovingDoor(parent, dpos, drot, dscale);

}

//�X�V����
void CSwitch::Update()
{
	//�s��̍X�V
	CTransform::Update();
}

//�t���O�̎擾
bool CSwitch::GetFlag()
{
	return mFlag;
}

//�Փˏ���
void CSwitch::Collision(CCollider* m, CCollider* o)
{
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��o���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			if (o->GetParent() != nullptr) {
				//�^�O���e�̎�
				if (o->GetParent()->GetTag() == CCharacter::ETag::EBULLET)
				{

					mFlag = true;

					//mEnabled = false;
				}
			}
		}
		break;
	}
}

//�Փˏ���
void CSwitch::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mCollider.ChangePriority();
	//�Փˏ����̎��s
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

