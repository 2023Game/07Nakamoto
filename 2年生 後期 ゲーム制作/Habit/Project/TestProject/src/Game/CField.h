#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CWall;

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	void Update();
	void Render();

private:
	// �ǂ𐶐�
	void CreateWalls();
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// �ǂ̃I�u�W�F�N�g�̃��X�g
	std::list<CWall*> mWalls;



	CModel* mpMap1;
};