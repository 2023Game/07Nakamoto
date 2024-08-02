#include "CColliderMesh.h"
#include "CCollider.h"

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
void CColliderMesh::ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model,CCollider::ETag tag)
{
	//モデルの三角ポリゴンで三角コライダの配列作成
	mpColliderTriangles = new
		CColliderTriangle[model->Triangles().size()];
	for (size_t i = 0; i < model->Triangles().size(); i++)
	{
		//タグの設定
		mpColliderTriangles[i].SetTag(tag);
		//三角コライダの設定
		mpColliderTriangles[i].SetColliderTriangle(parent, matrix
			, model->Triangles()[i].GetV0()
			, model->Triangles()[i].GetV1()
			, model->Triangles()[i].GetV2());

		//優先度の変更
		mpColliderTriangles[i].ChangePriority();
	}
}

//static変数の定義
CModel CColliderMesh::mFloor;
//床のモデルを取得する
CModel* CColliderMesh::GetModelFloor()
{
	return &mFloor;
}
//static変数の定義
CModel CColliderMesh::mObject;
//オブジェクトのモデルを取得する
CModel* CColliderMesh::GetModelObject()
{
	return &mObject;
}