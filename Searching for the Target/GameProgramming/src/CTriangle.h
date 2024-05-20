#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include "CVector.h"
#include "CMatrix.h"
/*
三角形クラス
*/

class CTriangle {
public:
	//マテリアル番号の取得
	int GetMaterialIdx();

	/// <summary>
	/// マテリアル番号の設定
	/// </summary>
	/// <param name="idx">マテリアル番号</param>
	void SetMaterialIdx(int idx);

	/// <summary>
	/// 頂点座標設定
	/// </summary>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="v2">頂点3</param>
	void SetVertex(const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// 法線設定
	/// </summary>
	/// <param name="n">法線ベクトル</param>
	void SetNormal(const CVector& n);

	/// <summary>
	/// 法線設定
	/// </summary>
	/// <param name="v0">法線ベクトル1</param>
	/// <param name="v1">法線ベクトル2</param>
	/// <param name="v2">法線ベクトル3</param>
	void SetNormal(const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// UV設定
	/// </summary>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="v2">頂点3</param>
	void SetUV(const CVector& v0, const CVector& v1, const CVector& v2);

	//頂点座標mV[0]の取得
	const CVector& GetV0() const;
	//頂点座標mV[1]の取得
	const CVector& GetV1() const;
	//頂点座標mV[2]の取得
	const CVector& GetV2() const;
	//法線mN[0]を取得
	const CVector& GetN0() const;
	//法線mN[1]を取得
	const CVector& GetN1() const;
	//法線mN[2]を取得
	const CVector& GetN2() const;
	//UV mUv[0]を取得
	const CVector& GetmU0() const;
	//UV mUv[1]を取得
	const CVector& GetmU1() const;
	//UV mUv[2]を取得
	const CVector& GetmU2() const;

	//描画
	void Render();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="m">行列</param>
	void Render(const CMatrix& m);

private:
	int mMaterialIdx;	//マテリアル番号

	CVector mV[3];	//頂点座標
	CVector mN[3];	//法線

	CVector mUv[3];	//テクスチャマッピング

};
#endif // !CTRIANGLE_H

