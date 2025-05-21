#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor;
class CLDoor;

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// ���̃R���C�_�[���擾
	CColliderMesh* GetFloorCol() const;
	// �ǂ̃R���C�_�[�̎擾
	CColliderMesh* GetWallCol() const;

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpFloor;
	CModel* mpWall;
	CModel* mpWallCol;

	CRDoor* mpRDoor;
	CLDoor* mpLDoor;

	CColliderMesh* mpFloorColliderMesh;
	CColliderMesh* mpWallColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};