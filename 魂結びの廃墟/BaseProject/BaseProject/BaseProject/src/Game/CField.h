#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// ���̃R���C�_�[���擾
	CColliderMesh* GetCollider() const;

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpFloor;
	CModel* mpWall;
	CModel* mpWallCol;
	CColliderMesh* mpFloorColliderMesh;
	CColliderMesh* mpWallColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};