#include "CField2.h"
#include "CBspMap2.h"
#include "CFloor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"
#include "CDoor.h"

#include "CNavNode.h"
#include "CNavManager.h"

CField2* CField2::spInstance = nullptr;

// フィールドのインスタンスを取得
CField2* CField2::Instance()
{
	return spInstance;
}

// コンストラクタ
CField2::CField2(int floorNum)
	: mpMapData(nullptr)

{
	spInstance = this;
	// ダンジョンの生成
	CreateDungeon();
}

// デストラクタ
CField2::~CField2()
{
	SAFE_DELETE(mpMapData);

	spInstance = nullptr;
}

void CField2::CreateDungeon()
{
	// リストを空にする
	if (mpMapData != nullptr)
	{
		// コライダーの削除
		//SAFE_DELETE(mpDungeonCollider);

		for (CObjectBase* obj : mpStageObjects) obj->Kill();

		//for (CFloor* floor : mpFloorObjects) floor->Kill();
		//for (CWall* wall : mpWallObjects) wall->Kill();
		//for (CPillar* cpillar : mpPillarObjects) cpillar->Kill();
		//for (CEntrance* entrance : mpEntranceObjects) entrance->Kill();
		//for (CDoor* door : mpDoorObjects) door->Kill();
		//for (CFloor* passegeFloor : mpPassegeObjects) passegeFloor->Kill();
		//for (CObjectBase* obj : mpObjects) obj->Kill();
		for (CColliderMesh* objcol : mpObjectColliders) objcol = nullptr; //TODO:nullptrでいいのか

		mpStageObjects.clear();
		mpFloorObjects.clear();
		mpWallObjects.clear();
		mpPillarObjects.clear();
		mpEntranceObjects.clear();
		mpDoorObjects.clear();
		mpPassegeObjects.clear();
		mpObjects.clear();
		mpObjectColliders.clear();

		SAFE_DELETE(mpMapData);

		//CNavManager::Instance()->RemoveCollider(GetWallCollider());
		//CItemManager::Instance()->AllRemoveItems();
		//CCrystalManager::Instance()->AllRemoveCrystals();

		// ノード管理クラスをリセット
		CNavManager::Instance()->Clear();
	}

	// ダンジョンの2Dデータを生成
	mpMapData = new CBspMap2(0);
	// ダンジョンデータを元にマップを生成
	CreateMap();

}

// マップの生成
void CField2::CreateMap()
{
	const auto& mapData = mpMapData->GetMapData();

	for (int y = 0; y < mapData.size(); y++)
	{
		for (int x = 0; x < mapData[y].size(); x++)
		{
			const auto& tile = mapData[y][x];

			if (tile.type == CBspMap2::TileType::eFloor)
			{
				// 床の生成
				CFloor* floor = new CFloor((CVector(x * TILE_SIZE, 50.0f, y * TILE_SIZE)));
				mpFloorObjects.push_back(floor);
				mpStageObjects.push_back(floor);
			}

			float haleTileSize = TILE_SIZE * 0.5f;

			if (tile.north == CBspMap2::EdgeType::eWall)
			{
				// 北の壁を生成
				SpawnWall(CVector(x * TILE_SIZE, 50.0f, y * TILE_SIZE - haleTileSize), GetWallRotation(Direction::North));
			}
			if (tile.south == CBspMap2::EdgeType::eWall)
			{
				// 南の壁を生成
				SpawnWall(CVector(x * TILE_SIZE, 50.0f, y * TILE_SIZE + haleTileSize), GetWallRotation(Direction::South));
			}
			if (tile.east == CBspMap2::EdgeType::eWall)
			{
				// 東の壁を生成
				SpawnWall(CVector(x * TILE_SIZE + haleTileSize, 50.0f, y * TILE_SIZE ), GetWallRotation(Direction::East));
			}
			if (tile.west == CBspMap2::EdgeType::eWall)
			{
				// 西の壁を生成
				SpawnWall(CVector(x * TILE_SIZE - haleTileSize, 50.0f, y * TILE_SIZE), GetWallRotation(Direction::West));
			}
		}
	}
}

// 壁の生成
void CField2::SpawnWall(const CVector& pos, float rotY)
{
	CWall* wall = new CWall(pos);
	wall->Rotation(0.0f, rotY, 0.0f);
	mpWallObjects.push_back(wall);
	mpStageObjects.push_back(wall);
}

// 方角で回転値を取得
float CField2::GetWallRotation(Direction dir)
{
	switch (dir)
	{
	case Direction::North: return 0.0f;		// 北
	case Direction::East:  return 90.0f;	// 東
	case Direction::South: return 180.0f;	// 南
	case Direction::West:  return 270.0f;	// 西
	}
	return 0.0f;
}


