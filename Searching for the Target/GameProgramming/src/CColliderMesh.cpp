#include "CColliderMesh.h"

//デフォルトコンストラクタ
CColliderMesh::CColliderMesh()
	:mpColliderTriangles(nullptr)
{

}

//デストラクタ
CColliderMesh::~CColliderMesh()
{
	if (mpColliderTriangles != nullptr)
	{
		//三角コライダ配列の削除
		delete[] mpColliderTriangles;
	}
}

//モデルから三角コライダの生成
void CColliderMesh::ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model)
{
	//モデルの三角ポリゴンで三角コライダの配列作成
	mpColliderTriangles = new
		CColliderTriangle[model->Triangles().size()];
	for (size_t i = 0; i < model->Triangles().size(); i++)
	{
		//三角コライダの設定
		mpColliderTriangles[i].SetColliderTriangle(parent, matrix
			, model->Triangles()[i].GetV0()
			, model->Triangles()[i].GetV1()
			, model->Triangles()[i].GetV2());
		mpColliderTriangles[i].ChangePriority();
	}
}

//優先度の変更