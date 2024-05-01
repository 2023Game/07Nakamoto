#include "CVector.h"

CVector::CVector()
	:mX(0.0f)
	,mY(0.0f)
	,mZ(0.0f)
{

}

CVector::CVector(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
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