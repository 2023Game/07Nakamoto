#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDungeonMap.h"
#include "CBpsMap.h"

class CFloor;
class CWall;
class CPillar;
class CEntrance;
class CDoor;
class CDungeonManeger;

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
	int ConvertDirectionAngle(CDungeonMap::Direction dir) const;
	// 方角によって回転値を設定
	int ConvertDirectionAngle2(CBpsMap::Direction dir) const;

	// 部屋の生成
	void CreateRoom();
	// BPMマップの部屋の生成
	void SetMapData(const std::vector<std::vector<CBpsMap::Tile>>& map);

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// ２次元配列のマップデータ保存用
	CDungeonManeger* mpMapData;

	CBpsMap* mpMapData2;
	//std::vector<std::vector<CBpsMap::Tile>> mMap;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;	// 床のリスト
	std::vector<CWall*> mpWallObjects;		// 壁のリスト
	std::vector<CPillar*> mpPillarObjects;	// 柱のリスト
	std::vector<CEntrance*> mpEntranceObjects;	// 出入口のリスト
	std::vector<CDoor*> mpDoorObjects;		// 扉のリスト
};