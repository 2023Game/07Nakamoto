#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// 床のコライダーを取得
	CCollider* GetCollider() const;

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CModel* mpModel2;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};