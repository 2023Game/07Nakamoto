#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// ���̃R���C�_�[���擾
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