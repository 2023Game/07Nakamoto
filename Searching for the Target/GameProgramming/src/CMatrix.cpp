#include "CMatrix.h"
//�W�����o�͊֐��̃C���N���[�h
#include <stdio.h>

//�~����M_PI��L���ɂ���
#define _USE_MATH_DEFINES
//���w�֐��̃C���N���[�h
#include <math.h>

void CMatrix::Print()
{
	printf("%10f%10f%10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f%10f%10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f%10f%10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f%10f%10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

//�f�t�H���g�R���X�g���N�^
CMatrix::CMatrix()
{
	Identity();
}

//�P�ɍs��̍쐬
CMatrix CMatrix::Identity()
{
	mM[0][0] = 1; mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = 1; mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = 1; mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;

	//���̍s���Ԃ�
	return *this;
}

//�g��k���s��̍쐬
CMatrix CMatrix::SetScale(float sx, float sy, float sz)
{
	mM[0][0] = sx;mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = sy;mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = sz;mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;

	//���̍s���Ԃ�
	return *this;
}
//�s��̎擾
float* CMatrix::GetM() const
{
	return (float*)mM[0];
}

//�s��l�̎擾
float CMatrix::GetM(int r, int c)const
{
	return mM[r][c];
}
//�s��l�̑��
void CMatrix::SetM(int row, int col, float value)
{
	mM[row][col] = value;
}

//��]�s��(X��)
CMatrix CMatrix::SetRotateX(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//X���ŉ�]����s��̐ݒ�
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//�s���Ԃ�
	return *this;
}

//��]�s��(Y��)
CMatrix CMatrix::SetRotateY(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Y���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//�s���Ԃ�
	return *this;
}

//��]�s��(Z��)
CMatrix CMatrix::SetRotateZ(float degree)
{
	//�p�x���烉�W�A�������߂�
	float rad = degree / 180.0f * M_PI;
	//�P�ʍs��ɂ���
	Identity();
	//Z���ŉ�]����s��̐ݒ�
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//�s���Ԃ�
	return *this;
}

//�ړ��s��
CMatrix CMatrix::SetTranslate(float mx, float my, float mz)
{
	mM[0][0] = 1;  mM[0][1] = 0;  mM[0][2] = 0;  mM[0][3] = 0;
	mM[1][0] = 0;  mM[1][1] = 1;  mM[1][2] = 0;  mM[1][3] = 0;
	mM[2][0] = 0;  mM[2][1] = 0;  mM[2][2] = 1;  mM[2][3] = 0;
	mM[3][0] = mx; mM[3][1] = my; mM[3][2] = mz; mM[3][3] = 1;
	//���̍s���Ԃ�
	return *this;
}

//�t�s��擾
CMatrix CMatrix::GetTranspose() const
{
	CMatrix t;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) 
		{
			t.mM[i][j] = mM[j][i];
		}
	}
	return t;
}

//*���Z�q�̃I�[�o�[���[�h
const CMatrix CMatrix::operator*(const CMatrix& m)const
{
	CMatrix t;
	for (int i = 0; i < 4; i++) 
	{
		t.mM[i][0] = mM[i][0] * m.mM[0][0] + mM[i][1] * m.mM[1][0] + mM[i][2] * m.mM[2][0] + mM[i][3] * m.mM[3][0];
		t.mM[i][1] = mM[i][0] * m.mM[0][1] + mM[i][1] * m.mM[1][1] + mM[i][2] * m.mM[2][1] + mM[i][3] * m.mM[3][1];
		t.mM[i][2] = mM[i][0] * m.mM[0][2] + mM[i][1] * m.mM[1][2] + mM[i][2] * m.mM[2][2] + mM[i][3] * m.mM[3][2];
		t.mM[i][3] = mM[i][0] * m.mM[0][3] + mM[i][1] * m.mM[1][3] + mM[i][2] * m.mM[2][3] + mM[i][3] * m.mM[3][3];
	}
	return t;
}
