#include "CVector.h"
#include <math.h>

CVector CVector::operator*(const float& f)const {
	return CVector(mX * f, mY * f, mZ * f);
}

CVector CVector::Normalize() const {
	//ベクトルの大きさで割ったベクトルを返す(長さ1のベクトル)
	return *this * (1.0f / Length());
}

//外積
CVector CVector::Cross(const CVector& v)const {
	return CVector(mY * v.mZ - mZ * v.mY, mZ * v.mX - mX * v.mZ, mX * v.mY - mY * v.mX);
}

//内積
float CVector::Dot(const CVector& v)const {
	return mX * v.mX + mY * v.mY + mZ * v.mZ;
}

//ベクトルの長さを返す
float CVector::Length() const {
	//sqrt関数で平方根を返す
	return sqrtf(mX * mX + mY * mY + mZ * mZ);
}

//Set(X座標,Y座標,Z座標)
void CVector::Set(float x, float y, float z)
{
	mX = x;
	mY = y;
	mZ = z;
}

void CVector::X(float f)
{
	mX = f;
}

void CVector::Y(float f)
{
	mY = f;
}

void CVector::Z(float f)
{
	mZ = f;
}

float CVector::X() const
{
	return mX;
}

float CVector::Y() const
{
	return mY;
}

float CVector::Z() const
{
	return mZ;
}

CVector::CVector()
	:mX(0.0f) 
	,mY(0.0f)
	,mZ(0.0f)
{}

CVector::CVector(float x, float y, float z)
	:mX(x)
	,mY(y)
	,mZ(z)
{}

CVector CVector::operator+=(const CVector& v)
{
	mX += v.mX;
	mY += v.mY;
	mZ += v.mZ;
	return *this;
}

CVector CVector::operator*(const CMatrix& m)const
{
	//掛け算の結果をCVector型の値で返す
	return CVector(
		mX * m.M(0, 0) + mY * m.M(1, 0) + mZ * m.M(2, 0) + m.M(3, 0),
		mX * m.M(0, 1) + mY * m.M(1, 1) + mZ * m.M(2, 1) + m.M(3, 1),
		mX * m.M(0, 2) + mY * m.M(1, 2) + mZ * m.M(2, 2) + m.M(3, 2)
	);
}

//+演算子のオーバーロード
//CVector + CVector の演算結果を返す
CVector CVector::operator+(const CVector & v) const
{
	return CVector(mX + v.mX, mY + v.mY, mZ + v.mZ);
}

//-演算子のオーバーロード
//CVector - CVector の演算結果を返す
CVector CVector::operator-(const CVector& v)const
{
	return CVector(mX - v.mX, mY - v.mY, mZ - v.mZ);
}

