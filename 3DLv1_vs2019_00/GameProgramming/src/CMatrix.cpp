#include "CMatrix.h"
#include "CVector.h"

//�W�����o�͊֐��̃C���N���[�h
#include <stdio.h>

float CMatrix::M(int r,int c)const
{
	return mM[r][c];
}

//�g��k���s��̍쐬
//Scale(�{��X,�{��Y,�{��Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz)
{
	for (int x = 0; x < 4; x++)
	{
		mM[x][0] = mM[x][0] * sx;
	}

	for (int y = 0; y < 4; y++)
	{
		mM[y][1] = mM[y][1] * sy;
	}

	for (int z = 0; z < 4; z++)
	{
		mM[z][2] = mM[z][2] * sz;
	}
	
	//���̍s���Ԃ�
	return *this;
}

//�f�t�H���g�R���X�g���N�^
CMatrix::CMatrix() {
	Identity();
}

//�P�ʍs��̍쐬
CMatrix CMatrix::Identity() {

	mM[0][0] = 1; mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = 1; mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = 1; mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;

	//���̍s���Ԃ�
	return *this;
}

void CMatrix::Print() {
	printf("%10f %10f %10f %10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f %10f %10f %10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f %10f %10f %10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f %10f %10f %10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

