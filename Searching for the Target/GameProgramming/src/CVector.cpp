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
{
	mX = x;
	mY = y;
	mZ = z;
}

//Set(X座標,Y座標,Z座標)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

//X座標の取得
float CVector::GetX() const
{
	return mX;
}

//Y座標の取得
float CVector::GetY() const
{
	return mY;
}

//Z座標の取得
float CVector::GetZ() const
{
	return mZ;
}

//ベクトルの長さを返す
float CVector::GetLength() const
{
	//sqro関数で平方根を返す
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

//+演算子のオーバーロード
//CVector + CVectorの演算結果を返す
CVector CVector::operator+(const CVector& v)const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

//-演算子のオーバーロード
//CVector - CVectorの演算結果を返す
CVector CVector::operator-(const CVector& v)const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

CVector CVector::operator*(const CMatrix& m)
{
	//掛け算の結果をCVector型の値で返す
	return CVector(
		mX * m.GetM(0, 0) + mY * m.GetM(1, 0) + mZ * m.GetM(2, 0) + m.GetM(3, 0),
		mX * m.GetM(0, 1) + mY * m.GetM(1, 1) + mZ * m.GetM(2, 1) + m.GetM(3, 1),
		mX * m.GetM(0, 2) + mY * m.GetM(1, 2) + mZ * m.GetM(2, 2) + m.GetM(3, 2)
	);
}