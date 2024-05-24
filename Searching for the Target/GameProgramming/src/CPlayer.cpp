#include "CPlayer.h"
#include "CTaskManager.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Y�̉�]���x
#define VELOCITY CVector(0.0f,0.0f,0.1f) //�ړ����x
#define VELOCITY_Y CVector(0.0f,2.0f,0.0f) //�W�����v���̈ړ���

//�W�����v�̉��̃t���O
//bool a = false;

//�f�t�H���g�R���X�g���N�^
CPlayer::CPlayer()
	: mLine(this, &mMatrix, CVector(0.0f, 0.3f, -1.5f), CVector(0.0f, 0.3f, 1.5f))
	, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -0.2f, 0.0f))
	, mLine3(this, &mMatrix, CVector(1.1f, 0.3f, 0.0f), CVector(-1.1f, 0.3f, 0.0f))
{
}

//�R���X�g���N�^
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);	//�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
	//D�L�[���͂ŉE��]
	if (mInput.Key('D'))
	{
		//Y���̉�]�l�����Z
		mRotation = mRotation - ROTATION_YV;
	}
	//A�L�[���͂ō���]
	if (mInput.Key('A'))
	{
		//Y���̉�]�l�����Z
		mRotation = mRotation + ROTATION_YV;
	}
	//W�L�[���͂őO�i
	if (mInput.Key('W'))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	//S�L�[���͂Ō��
	if (mInput.Key('S')) 
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition - VELOCITY * mMatrixRotate;
	}

	if (mInput.Key('K'))
	{
		mRotation = mRotation + CVector(1.0f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mRotation = mRotation - CVector(1.0f, 0.0f, 0.0f);
	}

	/* �Փ˔��肪�ł��Ă���W�����v�����\��
	//SPACE�L�[���͂ŃW�����v
	if (a == false) {
		if (mInput.Key(VK_SPACE))
		{
			//Y�������̒l���ړ�������
			mPosition = mPosition + VELOCITY_Y;
			a = true;
		}
	}

	if (a == true && !mInput.Key(VK_SPACE))
	{
		a = false;
	}
	*/

	//�ϊ��s��̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider* m, CCollider* o)
{
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->GetType())
	{
	case CCollider::EType::ELINE:	//���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//�ʒu�̍X�V
				mPosition = mPosition + adjust;
				//�s��̍X�V
				CTransform::Update();
			}
		}
		break;
	}
}