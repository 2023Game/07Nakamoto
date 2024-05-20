#include "CTriangle.h"
#include "glut.h"

//マテリアル番号の取得
int CTriangle::GetMaterialIdx()
{
	return mMaterialIdx;
}

//マテリアル番号の設定
void CTriangle::SetMaterialIdx(int idx)
{
	mMaterialIdx = idx;
}

//頂点設定
void CTriangle::SetVertex(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mV[0] = v0;
	mV[1] = v1;
	mV[2] = v2;
}

//法線設定
void CTriangle::SetNormal(const CVector& n)
{
	mN[0] = mN[1] = mN[2] = n;
}

//法線設定
void CTriangle::SetNormal(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mN[0] = v0;
	mN[1] = v1;
	mN[2] = v2;
}

//UV設定
void CTriangle::SetUV(const CVector& v0, const CVector& v1, const CVector& v2)
{
	mUv[0] = v0;
	mUv[1] = v1;
	mUv[2] = v2;
}

//頂点座標mV[0]の取得
const CVector& CTriangle::GetV0() const
{
	return mV[0];
}
//頂点座標mV[1]の取得
const CVector& CTriangle::GetV1() const
{
	return mV[1];
}
//頂点座標mV[2]の取得
const CVector& CTriangle::GetV2() const
{
	return mV[2];
}
//法線mN[0]の取得
const CVector& CTriangle::GetN0() const
{
	return mN[0];
}
//法線mN[1]の取得
const CVector& CTriangle::GetN1() const
{
	return mN[1];
}
//法線mN[2]の取得
const CVector& CTriangle::GetN2() const
{
	return mN[2];
}
//UV mUv[0]の取得
const CVector& CTriangle::GetmU0() const
{
	return mUv[0];
}
//UV mUv[1]の取得
const CVector& CTriangle::GetmU1() const
{
	return mUv[1];
}
//UV mUv[2]の取得
const CVector& CTriangle::GetmU2() const
{
	return mUv[2];
}

//描画
void CTriangle::Render()
{
	glBegin(GL_TRIANGLES);
	glNormal3f(mN[0].GetX(), mN[0].GetY(), mN[0].GetZ());
	glTexCoord2f(mUv[0].GetX(), mUv[0].GetY());
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glNormal3f(mN[1].GetX(), mN[1].GetY(), mN[1].GetZ());
	glTexCoord2f(mUv[1].GetX(), mUv[1].GetY());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glNormal3f(mN[2].GetX(), mN[2].GetY(), mN[2].GetZ());
	glTexCoord2f(mUv[2].GetX(), mUv[2].GetY());
	glVertex3f(mV[2].GetX(), mV[2].GetY(), mV[2].GetZ());
	glEnd();
}

//描画
void CTriangle::Render(const  CMatrix& m)
{
	CVector mV[3], mN[3];
	mV[0] = CTriangle::mV[0] * m;
	mV[1] = CTriangle::mV[1] * m;
	mV[2] = CTriangle::mV[2] * m;
	mN[0] = CTriangle::mN[0] * m;
	mN[1] = CTriangle::mN[1] * m;
	mN[2] = CTriangle::mN[2] * m;
	//法線は移動値を0
	CMatrix mat = m;
	mat.SetM(3, 0, 0.0f);
	mat.SetM(3, 1, 0.0f);
	mat.SetM(3, 2, 0.0f);
	mN[0] = CTriangle::mN[0] * mat;
	mN[1] = CTriangle::mN[1] * mat;
	mN[2] = CTriangle::mN[2] * mat;
	glBegin(GL_TRIANGLES);
	glNormal3f(mN[0].GetX(), mN[0].GetY(), mN[0].GetZ());
	glTexCoord2f(mUv[0].GetX(), mUv[0].GetY());
	glVertex3f(mV[0].GetX(), mV[0].GetY(), mV[0].GetZ());
	glNormal3f(mN[1].GetX(), mN[1].GetY(), mN[1].GetZ());
	glTexCoord2f(mUv[1].GetX(), mUv[1].GetY());
	glVertex3f(mV[1].GetX(), mV[1].GetY(), mV[1].GetZ());
	glNormal3f(mN[2].GetX(), mN[2].GetY(), mN[2].GetZ());
	glTexCoord2f(mUv[2].GetX(), mUv[2].GetY());
	glVertex3f(mV[2].GetX(), mV[2].GetY(), mV[2].GetZ());
	glEnd();
}