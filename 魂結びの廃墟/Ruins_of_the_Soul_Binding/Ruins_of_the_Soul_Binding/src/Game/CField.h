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

	// 床のコライダーを取得
	CColliderMesh* GetFloorCol() const;
	// 壁のコライダーの取得
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