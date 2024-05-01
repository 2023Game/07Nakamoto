#ifndef CMODEL_H 
#define CMODEL_H

#include "CTriangle.h"
#include "CMaterial.h"

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

private:
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
};

#endif // !CMODEL_H 
