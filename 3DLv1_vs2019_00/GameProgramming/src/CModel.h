#pragma once
#ifndef CMODEL_H
#define CMODEL_H
//vectorのインクルード
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel
{
public:
	const std::vector<CTriangle>& Triangles() const;
	//デストラクタの宣言
	~CModel();
	//モデルファイルの入力
	//Load(モデルファイル名,マテリアルファイル名)
	void Load(char* obj, char* mtl);
	//描画
	void Render();
	//描画
	//Render(行列)
	void Render(const CMatrix& m);

private:
	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*>mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
};

#endif
