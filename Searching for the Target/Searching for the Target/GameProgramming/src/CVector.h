#ifndef CVECTOR_H
#define CVECTOR_H

#include "CMatrix.h"
/*
ベクトルクラス
ベクトルデータを扱う
*/
class CVector {
public:
	//デフォルトコンストラクタ
	CVector();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="z">Z座標</param>
	CVector(float x, float y, float z);

	/// <summary>
	/// 各軸での値の設定
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="z">z座標</param>
	void Set(float x, float y, float z);

	//Xの値を得る
	float GetX() const;
	//Yの値を得る
	float GetY() const;
	//Zの値を得る
	float GetZ() const;

	//ベクトルの長さを返す
	float GetLength() const;

	//+演算子のオーバーロード
	//CVector + CVectorの演算結果を返す
	CVector operator+(const CVector& v)const;

	//-演算子のオーバーロード
	//CVector - CVectorの演算結果を返す
	CVector operator-(const CVector& v)const;

	//CVector * CMatrixの結果をCVectorで返す
	CVector operator*(const CMatrix& m);

private:
	//3D各軸での値を設定
	float mX, mY, mZ;
};

#endif // !CVECTOR_H

