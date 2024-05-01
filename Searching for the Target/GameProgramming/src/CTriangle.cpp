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