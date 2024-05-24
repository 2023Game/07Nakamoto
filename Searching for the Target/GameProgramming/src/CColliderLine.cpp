#include "CColliderLine.h"

//�R���X�g���N�^
CColliderLine::CColliderLine(CCharacter* parent, CMatrix* matrix,
	const CVector& v0, const CVector& v1)
{
	ColliderLineSet(parent, matrix, v0, v1);
}

//�����R���C�_�̐ݒ�
void CColliderLine::ColliderLineSet(CCharacter* parent, CMatrix* matrix,
	const CVector& v0, const CVector& v1)
{
	mType = EType::ELINE;	//�����R���C�_
	mpParent = parent;		//�e�ݒ�
	if (matrix)
		mpMatrix = matrix;	//�e�s�񂠂�ΐݒ�
	//�������_�ݒ�
	mV[0] = v0;
	mV[1] = v1;

}

void CColliderLine::Render()
{
	//�s��ޔ�
	glPushMatrix();
	//�s��K�p
	glMultMatrixf(mpMatrix->GetM());
	
	//�A���t�@�u�����h��L���ɂ���
	glEnable(GL_BLEND);
	//�u�����h���@��L���ɂ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//���C�g�I�t
	glDisable(GL_LIGHTING);

	//DIFFUSE�ԐF�ݒ�
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	glColor4fv(c);

	//�O�p�`�`��
	glBegin(GL_LINES);
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glEnd();

	//���C�g�I��
	glEnable(GL_LIGHTING);
	//�A���t�@�u�����h����
	glDisable(GL_ALPHA);
	//�s�񕜋A
	glPopMatrix();
}