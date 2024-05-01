#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include"CVector.h"
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

	//描画
	void Render();

private:
	int mMaterialIdx;	//マテリアル番号

	CVector mV[3];	//頂点座標
	CVector mN[3];	//法線

	CVector mUv[3];	//テクスチャマッピング

};
#endif // !CTRIANGLE_H

