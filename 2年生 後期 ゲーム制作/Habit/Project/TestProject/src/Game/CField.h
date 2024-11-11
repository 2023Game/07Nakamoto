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
	/// レイとフィールドオブジェクトの衝突判定
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
	// 壁を生成
	void CreateWalls();
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// 壁のオブジェクトのリスト
	std::list<CWall*> mWalls;



	CModel* mpMap1;
};