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
	BuildStage();
}

// マップの生成
void CField2::BuildStage()
{
	const auto& mapData = mpMapData->GetMapData();

	for (int y = 0; y < mapData.size(); y++)
	{
		for (int x = 0; x < mapData[y].size(); x++)
		{
			const auto& tile = mapData[y][x];

			float bx = x * TILE_SIZE;
			float bz = y * TILE_SIZE;

			if (tile.type == CBspMap2::TileType::eFloor)
			{
				CreateFloor((CVector(bx, 50.0f, bz)));
			}

			float haleTileSize = TILE_SIZE * 0.5f;

			if (tile.north == CBspMap2::EdgeType::eWall)
			{
				// 北の壁を生成
				CreateWall(CVector(bx, 50.0f, bz - haleTileSize), GetWallRotation(Direction::North));
			}
			if (tile.south == CBspMap2::EdgeType::eWall)
			{
				// 南の壁を生成
				CreateWall(CVector(bx, 50.0f, bz + haleTileSize), GetWallRotation(Direction::South));
			}
			if (tile.east == CBspMap2::EdgeType::eWall)
			{
				// 東の壁を生成
				CreateWall(CVector(bx + haleTileSize, 50.0f, bz), GetWallRotation(Direction::East));
			}
			if (tile.west == CBspMap2::EdgeType::eWall)
			{
				// 西の壁を生成
				CreateWall(CVector(bx - haleTileSize, 50.0f, bz), GetWallRotation(Direction::West));
			}

			// 北東に柱を生成
			if (tile.north == CBspMap2::EdgeType::eWall &&
				tile.east == CBspMap2::EdgeType::eWall)
			{
				CreatePillar(CVector(bx + haleTileSize, 50.0f, bz - haleTileSize));
			}
			// 北西に柱を生成
			if (tile.north == CBspMap2::EdgeType::eWall &&
				tile.west == CBspMap2::EdgeType::eWall)
			{
				CreatePillar(CVector(bx - haleTileSize, 50.0f, bz - haleTileSize));
			}
			// 南東に柱を生成
			if (tile.south == CBspMap2::EdgeType::eWall &&
				tile.east == CBspMap2::EdgeType::eWall)
			{
				CreatePillar(CVector(bx + haleTileSize, 50.0f, bz + haleTileSize));
			}
			// 南西に柱を生成
			if (tile.south == CBspMap2::EdgeType::eWall &&
				tile.west == CBspMap2::EdgeType::eWall)
			{
				CreatePillar(CVector(bx - haleTileSize, 50.0f, bz + haleTileSize));
			}

			// 2次元配列の範囲外だった場合、
			if (y - 1 < 0 || y + 1 > mapData.size() - 1 || x - 1 < 0|| x + 1 > mapData[y].size() - 1) continue;

			// 左のタイルに北の壁があり、
			// 上のタイルに西の壁がある場合、
			if (mapData[y][x - 1].north== CBspMap2::EdgeType::eWall &&
				mapData[y - 1][x].west == CBspMap2::EdgeType::eWall)
			{
				// 北西に柱を生成
				CreatePillar(CVector(bx - haleTileSize, 50.0f, bz - haleTileSize));
			}
			// 右のタイルに北の壁があり、
			// 上のタイルに東の壁がある場合、
			if (mapData[y][x + 1].north == CBspMap2::EdgeType::eWall &&
				mapData[y - 1][x].east == CBspMap2::EdgeType::eWall)
			{
				// 北東に柱を生成
				CreatePillar(CVector(bx + haleTileSize, 50.0f, bz - haleTileSize));
			}
			// 左のタイルに南の壁があり、
			// 下のタイルに西の壁がある場合、
			if (mapData[y][x - 1].south == CBspMap2::EdgeType::eWall &&
				mapData[y + 1][x].west == CBspMap2::EdgeType::eWall)
			{
				// 南西に柱を生成
				CreatePillar(CVector(bx - haleTileSize, 50.0f, bz + haleTileSize));
			}
			// 右のタイルに南の壁があり、
			// 下のタイルに東の壁がある場合、
			if (mapData[y ][x + 1].south == CBspMap2::EdgeType::eWall &&
				mapData[y + 1][x].east == CBspMap2::EdgeType::eWall)
			{
				// 南東に柱を生成
				CreatePillar(CVector(bx + haleTileSize, 50.0f, bz + haleTileSize));
			}

		}
	}
}

// 床の生成
void CField2::CreateFloor(const CVector& pos)
{
	// 床の生成
	CFloor* floor = new CFloor(pos);
	mpFloorObjects.push_back(floor);
	mpStageObjects.push_back(floor);
}

// 壁の生成
void CField2::CreateWall(const CVector& pos, float rotY)
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

// 柱を生成
void CField2::CreatePillar(const CVector& pos)
{
	CPillar* pillar = new CPillar(pos);
	mpPillarObjects.push_back(pillar);
	mpStageObjects.push_back(pillar);
}

// 通路の柱を生成
void CField2::CreatePassagePillar()
{
	const auto& map = mpMapData->GetMapData();
	int h = map.size();
	int w = map[0].size();

	for (int y = 1; y < h; y++)
	{
		for (int x = 1; x < w; x++)
		{
			int wallCount = 0;

			// 左上
			if (map[y - 1][x - 1].east == CBspMap2::EdgeType::eWall) wallCount++;
			if (map[y - 1][x - 1].south == CBspMap2::EdgeType::eWall) wallCount++;

			// 右上
			if (map[y - 1][x].west == CBspMap2::EdgeType::eWall) wallCount++;
			if (map[y - 1][x].south == CBspMap2::EdgeType::eWall) wallCount++;

			// 左下
			if (map[y][x - 1].east == CBspMap2::EdgeType::eWall) wallCount++;
			if (map[y][x - 1].north == CBspMap2::EdgeType::eWall) wallCount++;

			// 右下
			if (map[y][x].west == CBspMap2::EdgeType::eWall) wallCount++;
			if (map[y][x].north == CBspMap2::EdgeType::eWall) wallCount++;

			if (wallCount >= 2)
			{
				float px = x * TILE_SIZE - TILE_SIZE * 0.5f;
				float pz = y * TILE_SIZE - TILE_SIZE * 0.5f;
				CreatePillar(CVector(px, 50.0f, pz));
			}
		}
	}
}



