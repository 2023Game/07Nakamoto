#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDangeonMap.h"

class CFloor;
class CWall;
class CPillar;
class CDoor;
class Direction;

class CField : public CObjectBase
{
public:
	// コンストラクタ
	CField();
	// デストラクタ
	~CField();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	void CreateFieldObjects();

	// 方角によって回転値を設定
	int ConvertDirectionAngle(CDangeonMap::Direction dir) const;

	// 部屋の生成
	void CreateRoom();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// ２次元配列のマップデータ保存用
	CDangeonMap* mpMapData;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;	// 床のリスト
	std::vector<CWall*> mpWallObjects;		// 壁のリスト
	std::vector<CPillar*> mpCPillarObjects;	// 柱のリスト
	std::vector<CDoor*> mpDoorObjects;		// 扉のリスト
};