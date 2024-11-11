#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CWall;

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	/// <summary>
	/// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="hit"></param>
	/// <returns></returns>
	//bool CollisionRay(const CVector& start, const CVector& end,
	//	CHitInfo* hit) override;

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