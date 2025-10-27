#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CDungeonMap.h"
#include "CFloor.h"
#include "CDoor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"
#include "CDungeonManeger.h"
#include "CBspMap.h"

#define PILLAR_OFFSET_POS 10.0f	// 柱のオフセット座標

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	//CreateFieldObjects();

	// ダンジョン生成
	//mpMapData = new CDungeonManeger();
	//CreateRoom();

	// BPS法でダンジョン生成
	mpMapData2 = new CBspMap(50, 50);
	SetMapData(mpMapData2->GetTileData());

}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}

	//SAFE_DELETE(mpMapData);
	SAFE_DELETE(mpMapData2);
}

void CField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");

	new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(50.0f, 0.0f, 0.0f), 10.0f
	);
	new CRotateFloor
	(
		mpCylinderModel,
		CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
		1.0f
	);

	// 動かない床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動く床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 回転する床①
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// 動かない床③（坂道）
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// 電撃エフェクト
	CLineEffect* le = new CLineEffect(ETag::eField);
	le->SetTexture("LightningBolt");
	le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	le->SetAnimData(&mEffectAnimData);

	CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	int div = 3;
	float width = 5.0f;
	le->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		le->AddPoint(pos, width, width);
	}
}

// 方角によって回転値を設定
//int CField::ConvertDirectionAngle(CDungeonMap::Direction dir) const
//{
//	switch (dir)
//	{
//		// 北の場合
//		case CDungeonMap::Direction::eNorth:	return 0;
//		// 東の場合
//		case CDungeonMap::Direction::eEast:		return 90;
//		// 南の場合
//		case CDungeonMap::Direction::eSouth:	return 180;
//		// 西の場合
//		case CDungeonMap::Direction::eWest:		return 270;
//
//		default:	return 0;
//	}
//}

// 方角によって回転値を設定
int CField::ConvertDirectionAngle2(CBspMap::Direction dir) const
{
	switch (dir)
	{
		// 北の場合
	case CBspMap::Direction::eSouth:	return 0;
		// 東の場合
	case CBspMap::Direction::eWest:		return 90;
		// 南の場合
	case CBspMap::Direction::eNorth:	return 180;
		// 西の場合
	case CBspMap::Direction::eEast:		return 270;

	default:	return 0;
	}
}

// 部屋の生成
//void CField::CreateRoom()
//{
//	// 全体の区画の縦
//	for (int secY = 0; secY < DUNGEON_SECTION_Y; secY++)
//	{
//		// Z座標の区画のオフセット
//		int offSetPosZ = secY * ROOM_HEIGHT * TILE_SIZE;
//
//		// 全体の区画の横
//		for (int secX = 0; secX < DUNGEON_SECTION_X; secX++)
//		{
//			// X座標の区画のオフセット
//			int offSetPosX = secX * ROOM_WIDTH * TILE_SIZE;
//
//			// 区画を取得
//			const CDungeonMap* section = mpMapData->GetSection(secX, secY);
//
//			// 配列データの列
//			for (int y = 0; y < ROOM_HEIGHT; y++)
//			{
//				// 配列データの行
//				for (int x = 0; x < ROOM_WIDTH; x++)
//				{
//					// タイル情報の2次元配列のデータを取得
//					const CDungeonMap::Tile& tile = section->GetTile(x, y);
//
//					// 床の場合
//					if (tile.typeId == CDungeonMap::TileType::eFloor)
//					{
//						// 床の生成
//						CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 1, (y + 0.5f) *TILE_SIZE + offSetPosZ));	// コライダーが出来次第Y座標は0に変更
//						// 床のリストに追加
//						mpFloorObjects.push_back(floor);
//					}
//					// 壁の場合
//					else if (tile.typeId == CDungeonMap::TileType::eWall)
//					{
//						// 壁の生成
//						CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ));
//						// 壁の回転値を設定
//						int rotY = ConvertDirectionAngle(tile.dir);
//						wall->Rotation(0.0f, rotY, 0.0f);
//						// 壁のリストに追加
//						mpWallObjects.push_back(wall);
//					}
//					// 柱の場合
//					else if (tile.typeId == CDungeonMap::TileType::ePillar)
//					{
//						//方向に応じて座標を修正
//						CVector pillarPos = CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ);
//						// オフセットポジション格納用
//						CVector offSetPos;
//
//						// 北東の場合
//						if (tile.dir == CDungeonMap::Direction::eNorthEast)
//						{
//							offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
//						}
//						// 南東の場合
//						else if (tile.dir == CDungeonMap::Direction::eSouthEast)
//						{
//							offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
//						}
//						// 南西の場合
//						else if (tile.dir == CDungeonMap::Direction::eSouthWest)
//						{
//							offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
//						}
//						//北西の場合
//						else if (tile.dir == CDungeonMap::Direction::eNorthWest)
//						{
//							offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
//						}
//						pillarPos += offSetPos;
//
//						// 柱の生成
//						CPillar* pillar = new CPillar(pillarPos);
//						// 柱のリストに追加
//						mpPillarObjects.push_back(pillar);
//					}
//					// 出入口の場合
//					else if (tile.typeId == CDungeonMap::TileType::eEntrance)
//					{
//						// 出入口の生成
//						CEntrance* entrance = new CEntrance(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ));
//						int rotY = ConvertDirectionAngle(tile.dir);
//						entrance->Rotation(0.0f, rotY, 0.0f);
//						// 出入口のリストに追加
//						mpEntranceObjects.push_back(entrance);
//
//						// 床の生成
//						CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 1, (y + 0.5f) * TILE_SIZE + offSetPosZ));	// コライダーが出来次第Y座標は0に変更
//						// 床のリストに追加
//						mpFloorObjects.push_back(floor);
//
//						//CDoor* door = new CDoor(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ));
//						//door->Rotate(0.0f, rotY, 0.0f);
//						//// 扉のリストに追加
//						//mpDoorObjects.push_back(door);
//					}
//				}
//			}
//		}
//	}
//}

void CField::SetMapData(const std::vector<std::vector<CBspMap::Tile>>& map)
{
	for (int y = 0; y < map.size(); ++y)
	{
		for (int x = 0; x < map[y].size(); ++x)
		{
			const auto&tile = map[y][x];

			switch (map[y][x].type)
			{
			case CBspMap::TileType::eFloor:
			{
				// 床の生成
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
				// 床のリストに追加
				mpFloorObjects.push_back(floor);

				break;
			}
			case CBspMap::TileType::eWall:
			{
				// 壁の生成
				CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				// 壁の回転値を設定
				int rotY = ConvertDirectionAngle2(tile.dir);
				wall->Rotation(0.0f, rotY, 0.0f);
				// 壁のリストに追加
				mpWallObjects.push_back(wall);

				break;
			}
			case CBspMap::TileType::ePillar:
			{
				//方向に応じて座標を修正
				CVector pillarPos = CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE);
				// オフセットポジション格納用
				CVector offSetPos;

				// 北東の場合
				if (tile.dir == CBspMap::Direction::eNorthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				// 南東の場合
				else if (tile.dir == CBspMap::Direction::eSouthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				// 南西の場合
				else if (tile.dir == CBspMap::Direction::eSouthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				//北西の場合
				else if (tile.dir == CBspMap::Direction::eNorthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				pillarPos += offSetPos;

				// 柱の生成
				CPillar* pillar = new CPillar(pillarPos);
				// 柱のリストに追加
				mpPillarObjects.push_back(pillar);
				break;
			}
			case CBspMap::TileType::eEntrance:
			{
				// 出入口の生成
				CEntrance* entrance = new CEntrance(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				int rotY = ConvertDirectionAngle2(tile.dir);
				entrance->Rotation(0.0f, rotY, 0.0f);
				// 出入口のリストに追加
				mpEntranceObjects.push_back(entrance);

				// 床の生成
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
				// 床のリストに追加
				mpFloorObjects.push_back(floor);

				//CDoor* door = new CDoor(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ));
				//door->Rotate(0.0f, rotY, 0.0f);
				//// 扉のリストに追加
				//mpDoorObjects.push_back(door);
				break;
			}
			case CBspMap::TileType::ePassage:
			{
				// 床の生成
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
				// 床のリストに追加
				mpFloorObjects.push_back(floor);

				//// 北向きの場合
				//if (map[y][x].dir == CBspMap::Direction::eNorth)
				//{
				//	// 右の壁の生成
				//	CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				//	// 壁の回転値を設定
				//	wall->Rotation(0.0f, 90.0f, 0.0f);
				//	// 壁のリストに追加
				//	mpWallObjects.push_back(wall);
				//	// 左の壁の生成
				//	wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				//	// 壁の回転値を設定
				//	wall->Rotation(0.0f, 270.0f, 0.0f);
				//	// 壁のリストに追加
				//	mpWallObjects.push_back(wall);
				//}
				//// 東向きの場合
				//else if (map[y][x].dir == CBspMap::Direction::eEast)
				//{
				//	// 上の壁の生成
				//	CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				//	// 壁の回転値を設定
				//	wall->Rotation(0.0f, 180.0f, 0.0f);
				//	// 壁のリストに追加
				//	mpWallObjects.push_back(wall);
				//	// 下の壁の生成
				//	wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				//	// 壁の回転値を設定
				//	wall->Rotation(0.0f, 0.0f, 0.0f);
				//	// 壁のリストに追加
				//	mpWallObjects.push_back(wall);

				//}
				break;
			}
			default:
				break;
			}
		}
	}
}

// 更新
void CField::Update()
{
}

// 描画
void CField::Render()
{
	mpModel->Render(Matrix());

	// 床の描画
	for (CFloor* floor : mpFloorObjects)
	{
		floor->Render();
	}
	// 壁の描画
	for (CWall* wall : mpWallObjects)
	{
		wall->Render();
	}
	// 出入口の描画
	for (CEntrance* entrance : mpEntranceObjects)
	{
		entrance->Render();
	}
	// 柱の描画
	for (CPillar* pillar : mpPillarObjects)
	{
		pillar->Render();
	}
	// 扉の描画
	for (CDoor* door : mpDoorObjects)
	{
		door->Render();
	}

}
