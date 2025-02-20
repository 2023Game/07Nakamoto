#include "CMatrix.h"
//標準入出力関数のインクルード
#include <stdio.h>

//円周率M_PIを有効にする
#define _USE_MATH_DEFINES
//数学関数のインクルード
#include <math.h>

void CMatrix::Print()
{
	printf("%10f%10f%10f%10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f%10f%10f%10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f%10f%10f%10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f%10f%10f%10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

//デフォルトコンストラクタ
CMatrix::CMatrix()
{
	Identity();
}

//単に行列の作成
CMatrix CMatrix::Identity()
{
	mM[0][0] = 1; mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = 1; mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = 1; mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;

	//この行列を返す
	return *this;
}

//拡大縮小行列の作成
CMatrix CMatrix::SetScale(float sx, float sy, float sz)
{
	mM[0][0] = sx;mM[0][1] = 0; mM[0][2] = 0; mM[0][3] = 0;
	mM[1][0] = 0; mM[1][1] = sy;mM[1][2] = 0; mM[1][3] = 0;
	mM[2][0] = 0; mM[2][1] = 0; mM[2][2] = sz;mM[2][3] = 0;
	mM[3][0] = 0; mM[3][1] = 0; mM[3][2] = 0; mM[3][3] = 1;

	//この行列を返す
	return *this;
}
//行列の取得
float* CMatrix::GetM() const
{
	return (float*)mM[0];
}

//行列値の取得
float CMatrix::GetM(int r, int c)const
{
	return mM[r][c];
}
//行列値の代入
void CMatrix::SetM(int row, int col, float value)
{
	mM[row][col] = value;
}

//回転行列(X軸)
CMatrix CMatrix::SetRotateX(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//X軸で回転する行列の設定
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//行列を返す
	return *this;
}

//回転行列(Y軸)
CMatrix CMatrix::SetRotateY(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Y軸で回転する行列の設定
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//行列を返す
	return *this;
}

//回転行列(Z軸)
CMatrix CMatrix::SetRotateZ(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//行列を返す
	return *this;
}

//移動行列
CMatrix CMatrix::SetTranslate(float mx, float my, float mz)
{
	mM[0][0] = 1;  mM[0][1] = 0;  mM[0][2] = 0;  mM[0][3] = 0;
	mM[1][0] = 0;  mM[1][1] = 1;  mM[1][2] = 0;  mM[1][3] = 0;
	mM[2][0] = 0;  mM[2][1] = 0;  mM[2][2] = 1;  mM[2][3] = 0;
	mM[3][0] = mx; mM[3][1] = my; mM[3][2] = mz; mM[3][3] = 1;
	//この行列を返す
	return *this;
}

//逆行列取得
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

//*演算子のオーバーロード
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
