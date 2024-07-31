#include "CSwitch.h"
#include "CCollisionManager.h"

//�f�t�H���g�R���X�g���N�^
CSwitch::CSwitch()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 1.5f)
	, mFlag(false)
{
	mCollider.SetTag(CCollider::ETag::ESWITCH);
}

//�X�C�b�`�̐���
void CSwitch::SetSwitch(CModel* model, const CVector& pos,
	const CVector& rot, const CVector& scale)
{
	mpModel = model;		//���f��
	mPosition = pos;		//�ʒu
	mRotation = rot;		//��]
	mScale = scale;			//�g�k

	//�s��̍X�V
	CTransform::Update();

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
				if (o->GetTag() == CCollider::ETag::EBULLET)
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

//static�ϐ��̒�`
CModel CSwitch::mModelSwitch;

//�X�C�b�`�̃��f�����擾����
CModel* CSwitch::GetModelSwitch()
{
	return &mModelSwitch;
}
