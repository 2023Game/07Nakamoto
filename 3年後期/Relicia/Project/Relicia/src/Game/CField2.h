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

	// 方角
	enum class Direction
	{
		North,	// 北
		East,	// 東
		South,	// 南
		West	// 西
	};

	// ダンジョン生成
	void CreateDungeon();

private:
	// フィールドのインスタンス
	static CField2* spInstance;

	// マップの生成
	void BuildStage();
	// 床の生成
	void CreateFloor(const CVector& pos);
	// 壁の生成
	void CreateWall(const CVector& pos, float rotY);
	// 方角で回転値を取得
	float GetWallRotation(Direction dir);
	// 柱を生成
	void CreatePillar(const CVector& pos);
	// 通路の柱を生成
	void CreatePassagePillar();

	// ダンジョンの2次元配列データ
	CBspMap2* mpMapData;

	// それぞれの3Dオブジェクトのリスト
	std::vector<CFloor*> mpFloorObjects;		// 床のリスト
	std::vector<CWall*> mpWallObjects;			// 壁のリスト
	std::vector<CPillar*> mpPillarObjects;		// 柱のリスト
	std::vector<CEntrance*> mpEntranceObjects;	// 出入口のリスト
	std::vector<CDoor*> mpDoorObjects;			// 扉のリスト
	std::vector<CFloor*> mpPassegeObjects;		// 通路の床リスト

	std::vector<CObjectBase*> mpObjects;			// 遮蔽物になるオブジェクトのリスト
	std::vector<CColliderMesh*> mpObjectColliders;	// 遮蔽になるオブジェクトのコライダーリスト

	std::vector<CObjectBase*> mpStageObjects;	// 生成されたオブジェクトのリスト(削除用)
};