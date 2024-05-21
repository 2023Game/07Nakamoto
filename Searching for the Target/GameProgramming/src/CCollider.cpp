#include "CCollider.h"
#include "CCollisionManager.h"

//�f�t�H���g�R���X�g���N�^
CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mRadius(0)
{
	//�R���W�����}�l�[�W���ɒǉ�
	CCollisionManager::GetInstance()->Add(this);
}

//�R���X�g���N�^
CCollider::CCollider(CCharacter* parent, CMatrix* matrix,
	const CVector& position, float radius) 
	:CCollider() 
{
	
	//�e�ݒ�
	mpParent = parent;
	//�e�s��ݒ�
	mpMatrix = matrix;
	//CTransform�ݒ�
	mPosition = position;	//�ʒu
	//���a�ݒ�
	mRadius = radius;
}

//�f�X�g���N�^
CCollider::~CCollider()
{
	//�R���W�������X�g����폜
	CCollisionManager::GetInstance()->Remove(this);
}

//�e�|�C���^�̎擾
CCharacter* CCollider::GetParent()
{
	return mpParent;
}
//�`��
void CCollider::Render()
{
	glPushMatrix();
	//�R���C�_�̒��S���W���v�Z
	//�����̍��W�~�e�̕ϊ��s����|����
	CVector pos = mPosition * *mpMatrix;
	//���S���W�ֈړ�
	glMultMatrixf(CMatrix().SetTranslate(pos.GetX(), pos.GetY(), pos.GetZ()).GetM());
	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//���`��
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

//�Փ˔���
bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//�e�R���C�_�̒��S���W�����߂�
	//���_�~�R���C�_�̕ϊ��s��~�ϊ��s��
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//���S���璆�S�x�N�g�������߂�
	mpos = mpos - opos;
	//���S�̋��������a�̍��v��菬�����ƏՓ�
	if (m->mRadius + o->mRadius > mpos.GetLength())
	{
		//�Փ˂��Ă���
		return true;
	}
	//�Փ˂��Ă��Ȃ�
	return false;
}