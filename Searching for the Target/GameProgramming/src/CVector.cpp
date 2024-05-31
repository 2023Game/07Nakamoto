#include "CVector.h"
#include "CMatrix.h"
#include <math.h>

CVector::CVector()
	:mX(0.0f)
	,mY(0.0f)
	,mZ(0.0f)
{

}

CVector::CVector(float x, float y, float z)
	: mX(x)
	, mY(y)
	, mZ(z)
{
	
}

//Set(X���W,Y���W,Z���W)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

//X���W�̎擾
float CVector::GetX() const
{
	return mX;
}

//Y���W�̎擾
float CVector::GetY() const
{
	return mY;
}

//Z���W�̎擾
float CVector::GetZ() const
{
	return mZ;
}

//�x�N�g���̒�����Ԃ�
float CVector::GetLength() const
{
	//sqro�֐��ŕ�������Ԃ�
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

//����
float CVector::Dot(const CVector& v) const
{
	return mX * v.mX + mY * v.mY + mZ * v.mZ;
}

//�O��
CVector CVector::Cross(const CVector& v)const
{
	return CVector(mY * v.mZ - mZ * v.mY, mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX);
}

//���K��
CVector CVector::Nomalize() const 
{
	//�x�N�g���̑傫���Ŋ������x�N�g����Ԃ�(����1�̃x�N�g��)
	return *this * (1.0f / GetLength());
}

//*���Z�q�̃I�[�o�[���[�h
//CVector * float �̉��Z���ʂ�Ԃ�
CVector CVector::operator*(const float& f)const
{
	return CVector(mX * f, mY * f, mZ * f);
}

//+���Z�q�̃I�[�o�[���[�h
//CVector + CVector�̉��Z���ʂ�Ԃ�
CVector CVector::operator+(const CVector& v)const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

//-���Z�q�̃I�[�o�[���[�h
//CVector - CVector�̉��Z���ʂ�Ԃ�
CVector CVector::operator-(const CVector& v)const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

CVector CVector::operator*(const CMatrix& m)
{
	//�|���Z�̌��ʂ�CVector�^�̒l�ŕԂ�
	return CVector(
		mX * m.GetM(0, 0) + mY * m.GetM(1, 0) + mZ * m.GetM(2, 0) + m.GetM(3, 0),
		mX * m.GetM(0, 1) + mY * m.GetM(1, 1) + mZ * m.GetM(2, 1) + m.GetM(3, 1),
		mX * m.GetM(0, 2) + mY * m.GetM(1, 2) + mZ * m.GetM(2, 2) + m.GetM(3, 2)
	);
}