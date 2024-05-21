#include "CBullet.h"
#include "CPlayer.h"

//�f�t�H���g�R���X�g���N�^
CBullet::CBullet()
	: mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.5f)
{

}

//���Ɖ��s��
void CBullet::SetSize(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(5.0f, 5.0f, 5.0f);
	//�O�p�`�̒��_�ݒ�
	mT.SetVertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//�O�p�`�̖@���ݒ�
	mT.SetNormal(CVector(0.0f, 1.0f, 0.0f));
}

//�Փˏ���
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//�R���C�_��m��o���Փ˂��Ă��邩����
	if (CCollider::Collision(m, o))
	{
		//�Փ˂��Ă���Ƃ��͖����ɂ���
		mEnabled = false;
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
		mPosition = mPosition + CVector(-2.0f, 0.0f, 0.0f) * mMatrixRotate;
	}
	else
	{
		//�����ɂ���
		mEnabled = false;
	}
	
}