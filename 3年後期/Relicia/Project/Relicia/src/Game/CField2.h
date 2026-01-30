#pragma once
#include "CObjectBase.h"

class CBspMap2;
class CFloor;
class CWall;
class CPillar;
class CEntrance;
class CDoor;
class CCrystalObj;
class CItemObj;
class CEscapeArea;
class CColliderMesh;
class CBspMapCollider;

// フィールドクラス
class CField2 : public CObjectBase
{
public:
	// フィールドのインスタンスを取得
	static CField2* Instance();

	// コンストラクタ
	CField2(int floorNum);
	// デストラクタ
	~CField2();

	// ダンジョン生成
	void CreateDungeon();
	// マップの生成
	void CreateMap();

private:
	// フィールドのインスタンス
	static CField2* spInstance;

	// ダンジョンの2次元配列データ
	CBspMap2* mpMapData;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;	// 床のリスト
	std::vector<CWall*> mpWallObjects;		// 壁のリスト
	std::vector<CPillar*> mpPillarObjects;	// 柱のリスト
	std::vector<CEntrance*> mpEntranceObjects;	// 出入口のリスト
	std::vector<CDoor*> mpDoorObjects;		// 扉のリスト
	std::vector<CFloor*> mpPassegeObjects;	// 通路の床リスト
	std::vector<CVector> mNavNodePositions;	// NavNodeを配置する座標のリスト
	std::vector<CNavNode*> mpNavNodes;		// NavNodeリスト
	std::vector<CObjectBase*> mpObjects;	// 遮蔽物になるオブジェクトのリスト
	std::vector<CColliderMesh*> mpObjectColliders;	// 遮蔽になるオブジェクトのコライダーリスト

};