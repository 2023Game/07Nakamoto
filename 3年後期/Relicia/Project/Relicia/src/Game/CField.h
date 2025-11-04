#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDungeonMap.h"
#include "CBspMap.h"

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
	// ダンジョンマップの生成
	void CreateMap();

	void CreateFieldObjects();

	// 方角によって回転値を設定
	int ConvertDirectionAngle(CBspMap::Direction dir) const;
	// BPMマップのダンジョンの生成
	void SetMapData(const std::vector<std::vector<CBspMap::Tile>>& map);

	// 通路の壁の生成
	void CreatePassageWall(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
	
	// ２次元配列のマップデータ保存用
	CBspMap* mpMapData;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;	// 床のリスト
	std::vector<CWall*> mpWallObjects;		// 壁のリスト
	std::vector<CPillar*> mpPillarObjects;	// 柱のリスト
	std::vector<CEntrance*> mpEntranceObjects;	// 出入口のリスト
	std::vector<CDoor*> mpDoorObjects;		// 扉のリスト

	CColliderTriangle* mpCol;
};