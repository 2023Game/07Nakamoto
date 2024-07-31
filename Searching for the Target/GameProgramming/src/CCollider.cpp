#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

//�~����M_PI��L���ɂ���
#define _USE_MATH_DEFINES
//���w�֐��̃C���N���[�h
#include <math.h>

//�f�t�H���g�R���X�g���N�^
CCollider::CCollider()
	: mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mTag(ETag::ENULL)
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
//�^�C�v�̐ݒ�
mType = EType::ESPHERE;
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
//�R���C�_�^�C�v�̎擾
CCollider::EType CCollider::GetType()
{
	return mType;
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

CVector CCollider::GetNormal(CCollider* t)
{
	//�O�p�R���C�_�̖@�������߂�
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector TYvector = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();

	return TYvector;
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

//�O�p�`�Ƌ��̏Փ˔���
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();
	//���R���C�_�����[���h���W�ō쐬
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(nullptr, nullptr, sv, ev);
	//�O�p�R���C�_�Ɛ��R���C�_�̏Փˏ���
	return CollisionTriangleLine(t, &line, a);

}

//���ƎΖʂ̏Փˏ���
CVector CCollider::Slope(CCollider* p, CCollider* t, CVector* a)
{
	//�O�p�R���C�_�̖@�������߂�
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector TYvector = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();

	//printf("�@��:%10f %10f %10f\n", TYvector.GetX(), TYvector.GetY(), TYvector.GetZ());
	
	//�v���C���[��Z�������̃x�N�g�������߂�
	CVector PZvector = CVector(0, 0, 1) * p->mpParent->GetMatrixRotate();
	//�v���C���[��X�������̃x�N�g�������߂�
	CVector PXvector = TYvector.Cross(PZvector).Nomalize();

	//�v���C���[��Z�������̃x�N�g�������߂�
	CVector TZvector = PXvector.Cross(TYvector).Nomalize();
	
	double rx, ry, rz;	//���߂����W�A�����i�[
	float x, y, z;		//���߂��x�����i�[

	//X���̉�]�l(���W�A��)�����߂�
	rx = -asin(TZvector.GetY());
	//Y���̉�]�l(���W�A��)�����߂�
	ry = atan2(TZvector.GetX(), TZvector.GetZ());
	//Z���̉�]�l(���W�A��)�����߂�
	rz = atan2(PXvector.GetY(), TYvector.GetY());

	//���W�A������p�x�����߂�
	x = rx * 180 / M_PI;
	y = ry * 180 / M_PI;
	z = rz * 180 / M_PI;
	
	//��̊p�x
	*a = CVector(x, y, z);

	return *a;

	/*
	printf("%10f\n", a->GetX());
	printf("%10f\n", ajust.GetX());
	*/

	//�m�F�p
	//printf("���W�A�� : %10f %10f %10f\n", rx, ry, rz);
	//printf("�x��     : %10f %10f %10f\n", x, y, z);
}


//�O�p�`�Ɛ����̏Փ˔���
bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a)
{
	CVector v[3], sv, ev;
	//�e�R���C�_�̒��_�����[���h���W�֕ϊ�
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//�ʂ̖@�����A�O�ς𐳋K�����ċ��߂�
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Nomalize();
	//�O�p�`�̒��_��������n�_�ւ̃x�N�g�������߂�
	CVector v0sv = sv - v[0];
	//�O�p�̒��_��������I�_�ւ̃x�N�g�������߂�
	CVector v0ev = ev - v[0];
	//�������ʂƌ������Ă��邩���ςŊm�F����
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//�v���X�͌������Ă��Ȃ�
	if (dots * dote >= 0.0f)
	{
		//�Փ˂��ĂȂ�(�����s�v)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//�����͖ʂƌ������Ă���

	//�ʂƐ����̍D�]�����߂�
	//��_�̌v�Z
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//��_���O�p�`���Ȃ�Փ˂��Ă���
	//���_1,���_2�x�N�g���ƒ��_1��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̂Ȃ��Ȃ��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_2���_3�x�N�g���ƒ��_2��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//���_3���_1�x�N�g���ƒ��_3��_�x�N�g���Ƃ̊O�ς����߁A
	//�@���Ƃ̓��ς��}�C�i�X�Ȃ�A�O�p�`�̊O
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f)
	{
		//�Փ˂��ĂȂ�
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//�����l�v�Z(�Փ˂��Ȃ��ʒu�܂Ŗ߂�)
	if (dots < 0.0f)
	{
		//�n�_������
		*a = normal * -dots;
	}
	else
	{
		//�I�_������
		*a = normal * -dote;
	}
	return true;
}

//�D��x�̕ύX
void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::GetInstance()->Remove(this);	//��U�폜
	CCollisionManager::GetInstance()->Add(this);	//�ǉ�
}

//�D��x�̕ύX
void CCollider::ChangePriority()
{
	//�����̍��W�~�e�̕ϊ��s����|���ă��[���h���W�����߂�
	CVector pos = mPosition * *mpMatrix;
	//�x�N�g���̒������D��x
	CCollider::ChangePriority(pos.GetLength());
}

//�^�O�̐ݒ�
void CCollider::SetTag(ETag tag)
{
	mTag = tag;
}

//�^�O�̎擾
CCollider::ETag CCollider::GetTag()
{
	return mTag;
}
