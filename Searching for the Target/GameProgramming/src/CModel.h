#ifndef CMODEL_H 
#define CMODEL_H

#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

//vectorのインクルード
#include <vector>

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel
{
public:
	//デストラクタ
	~CModel();
	//モデルファイルの入力
	void Load(char* obj, char* mtl);
	//描画
	void Render();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="m">行列</param>
	void Render(const CMatrix& m);

	const std::vector<CTriangle>& Triangles() const;

private:
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;

	//頂点の配列
	CVertex* mpVertexes;
	void CreateVertexBuffer();
};

#endif // !CMODEL_H 
