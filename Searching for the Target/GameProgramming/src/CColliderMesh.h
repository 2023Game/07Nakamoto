#ifndef CCOLLIDERMESH_H
#define CCOLLIDERMESH_H

#include "CModel.h"
#include "CColliderTriangle.h"
#include "CCollider.h"


/*
モデルデータから三角コライダの生成
*/
class CColliderMesh
{
	friend CCollider;
public:
	//コンストラクタ
	CColliderMesh();
	//デストラクタ
	~CColliderMesh();

	/// <summary>
	/// モデルから三角コライダを設定
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="model">モデル</param>
	void ColliderMeshSet(CCharacter* parent, CMatrix* matrix, CModel* model
								, CCollider::ETag tag = CCollider::ETag::ENULL);

	static CModel* GetModelFloor();
	static CModel* GetModelObject();

private:
	//三角コライダの配列作成
	CColliderTriangle* mpColliderTriangles;

	static CModel mFloor;
	static CModel mObject;
};

#endif // !CCOLLIDERMESH_H
