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
class CBspMapCollider;

class CField : public CObjectBase
{
public:
	// フィールドのインスタンスを返す
	static CField* Instance();

	// コンストラクタ
	CField();
	// デストラクタ
	~CField();

	// 2次元配列のダンジョンデータを取得
	CBspMap* GetMapData() const;
	
	// 床タイルのランダムな座標を取得
	//const CVector GetRandomFloorPos() const;

	// 床コライダーを取得
	CColliderMesh* GetFloorCollider() const;
	// 壁コライダーを取得
	CColliderMesh* GetWallCollider() const;

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
	// 方角によってオフセット座標を設定
	CVector2 CvnvertDirectionPos(CBspMap::Direction dir) const;

	// BPMマップのダンジョンの生成
	void CreateDungeon(const std::vector<std::vector<CBspMap::Tile>>& map);

	// 通路の壁の生成
	void CreatePassageWall(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);
	// 通路の柱を生成
	void CreatePassagePillar(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	// フィールドのインスタンス
	static CField* spInstance;

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
	std::vector<CFloor*> mpPassegeObjects;	// 通路の床リスト

	// コライダーのポインタ
	CBspMapCollider* mpDungeonCollider;

};