#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CFloor.h"
#include "CDoor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"
#include "CBspMap.h"

#include "Maths.h"
#include "CDebugInput.h"
#include "CBspMapCollider.h"
#include "CCrystalObj.h"

#define PILLAR_OFFSET_POS 10.0f	// 柱のオフセット座標
#define SECTION_SIZE_X 50		// ダンジョンの全体の区画の横サイズ
#define SECTION_SIZE_Y 50		// ダンジョンの全体の区画の縦サイズ

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
	, mpMapData(nullptr)
	, mpDungeonCollider(nullptr)

{
	// BSP法でダンジョン生成
	CreateMap();


	// 属性クリスタルを生成
	CCrystalObj* crystal = new CCrystalObj(ElementType::Fire, mpMapData->GetRoomFloorPos());
}

CField::~CField()
{
	SAFE_DELETE(mpMapData);
	// コライダーの削除
	SAFE_DELETE(mpDungeonCollider);
}

// 2次元配列のダンジョンデータを取得
const CBspMap* CField::GetMapData() const
{
	return mpMapData;
}

// 床タイルのランダムな座標を取得
const CVector CField::GetRandomFloorPos() const
{
	if (!mpFloorObjects.empty())
	{
		int index = Math::Rand(0, mpFloorObjects.size() - 1);

		return mpFloorObjects[index]->Position();
	}
}

// 床コライダーを取得
CColliderMesh* CField::GetFloorCollider() const
{
	return mpDungeonCollider->GetFloorCollider();
}

// 壁コライダーを取得
CColliderMesh* CField::GetWallCollider() const
{
	return mpDungeonCollider->GetWallCollider();
}


// ダンジョンマップを生成
void CField::CreateMap()
{
	if (mpMapData != nullptr)
	{
		SAFE_DELETE(mpMapData);
		// コライダーの削除
		SAFE_DELETE(mpDungeonCollider);

		for (CFloor* floor : mpFloorObjects) floor->Kill();
		for (CWall* wall : mpWallObjects) wall->Kill();
		for (CPillar* cpillar : mpPillarObjects) cpillar->Kill();
		for (CEntrance* entrance : mpEntranceObjects) entrance->Kill();
		for (CDoor* door : mpDoorObjects) door->Kill();
		for (CFloor* passegeFloor : mpPassegeObjects) passegeFloor->Kill();

		mpFloorObjects.clear();
		mpWallObjects.clear();
		mpPillarObjects.clear();
		mpEntranceObjects.clear();
		mpDoorObjects.clear();
		mpPassegeObjects.clear();
	}

	// BSP法のダンジョンデータを生成
	mpMapData = new CBspMap(SECTION_SIZE_X, SECTION_SIZE_Y);
	// BSP法のダンジョン生成
	CreateDungeon(mpMapData->GetTileData());

	// ダンジョンのコライダーの生成
	mpDungeonCollider = new CBspMapCollider(mpMapData, SECTION_SIZE_X, SECTION_SIZE_Y);

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
int CField::ConvertDirectionAngle(CBspMap::Direction dir) const
{
	switch (dir)
	{
		// 北の場合
	case CBspMap::Direction::eNorth:	return 0;
		// 東の場合
	case CBspMap::Direction::eEast:		return 90;
		// 南の場合
	case CBspMap::Direction::eSouth:	return 180;
		// 西の場合
	case CBspMap::Direction::eWest:		return 270;

	default:	return 0;
	}
}

// 方角によってオフセット座標を設定
CVector2 CField::CvnvertDirectionPos(CBspMap::Direction dir) const
{
	switch (dir)
	{
		// 北の場合
	case CBspMap::Direction::eNorth:	return CVector2(0, 1);
		// 東の場合
	case CBspMap::Direction::eEast:		return CVector2(-1, 0);
		// 南の場合
	case CBspMap::Direction::eSouth:	return CVector2(0, -1);
		// 西の場合
	case CBspMap::Direction::eWest:		return CVector2(1, 0);

	default:	return CVector2(0, 0);
	}
}

// BPMマップのダンジョンの生成
void CField::CreateDungeon(const std::vector<std::vector<CBspMap::Tile>>& map)
{
	for (int y = 0; y < map.size(); ++y)
	{
		for (int x = 0; x < map[y].size(); ++x)
		{
			const auto&tile = map[y][x];

			switch (map[y][x].type)
			{
				// 床
				case CBspMap::TileType::eFloor:
				{
					// 床の生成
					CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
					// 床のリストに追加
					mpFloorObjects.push_back(floor);

					break;
				}
				// 壁
				case CBspMap::TileType::eWall:
				{
					// 壁の回転値とオフセット座標を設定
					int rotY = ConvertDirectionAngle(tile.dir);
					CVector2 pos = CvnvertDirectionPos(tile.dir);

					// 壁の生成
					CWall* wall = new CWall(CVector((x + pos.X()) * TILE_SIZE, 0, (y + pos.Y()) * TILE_SIZE));
					wall->Rotation(0.0f, rotY, 0.0f);
					// 壁のリストに追加
					mpWallObjects.push_back(wall);

					// 通路だったら、床も生成する
					if (map[y][x].passage)
					{
						// 床の生成
						CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
						// 床のリストに追加
						mpFloorObjects.push_back(floor);
					}
					break;
				}
				// 柱
				case CBspMap::TileType::ePillar:
				{
					//方向に応じて座標を修正
					CVector pillarPos = CVector(x * TILE_SIZE, 0, y * TILE_SIZE);
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

					// 通路だったら、床も生成する
					if (map[y][x].passage)
					{
						// 床の生成
						CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
						// 床のリストに追加
						mpFloorObjects.push_back(floor);
					}
					break;
				}
				// 出入口
				case CBspMap::TileType::eEntrance:
				{
					// 壁の回転値とオフセット座標を設定
					int rotY = ConvertDirectionAngle(tile.dir);
					CVector2 pos = CvnvertDirectionPos(tile.dir);

					// 出入口の生成
					CEntrance* entrance = new CEntrance(CVector((x + pos.X()) * TILE_SIZE, 0, (y + pos.Y()) * TILE_SIZE));
					entrance->Rotation(0.0f, rotY, 0.0f);
					// 出入口のリストに追加
					mpEntranceObjects.push_back(entrance);

					// 床の生成
					CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
					// 床のリストに追加
					mpPassegeObjects.push_back(floor);

					// 扉の生成
					//CDoor* door = new CDoor(CVector(x  * TILE_SIZE + offSetPosX, 0, y * TILE_SIZE + offSetPosZ));
					//door->Rotate(0.0f, rotY, 0.0f);
					//// 扉のリストに追加
					//mpDoorObjects.push_back(door);
					break;
				}
				// 通路
				case CBspMap::TileType::ePassage:
				{
					// 床の生成
					CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));	// コライダーが出来次第Y座標は0に変更
					// 床のリストに追加
					mpPassegeObjects.push_back(floor);

					break;
				}
			}

			// 通路フラグがtrueなら
			if (map[y][x].passage)
			{
				// 通路の壁を生成
				CreatePassageWall(map, x, y);

				CreatePassagePillar(map, x, y);
			}
		}
	}
}

// 通路の壁の生成
void CField::CreatePassageWall(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y)
{
	// 上のタイルを確認
	if (!map[y - 1][x].passage && map[y - 1][x].type != CBspMap::TileType::eFloor)
	{
		// 壁の生成
		CWall* wall = new CWall(CVector(x  * TILE_SIZE, 0, y  * TILE_SIZE));
		// 壁の回転値を設定
		int rotY = ConvertDirectionAngle(CBspMap::Direction::eNorth);
		wall->Rotation(0.0f, rotY, 0.0f);
		// 壁のリストに追加
		mpWallObjects.push_back(wall);
	}
	// 右のタイルを確認
	if (!map[y][x + 1].passage && map[y][x + 1].type != CBspMap::TileType::eFloor)
	{
		// 壁の生成
		CWall* wall = new CWall(CVector(x  * TILE_SIZE, 0, y  * TILE_SIZE));
		// 壁の回転値を設定
		int rotY = ConvertDirectionAngle(CBspMap::Direction::eEast);
		wall->Rotation(0.0f, rotY, 0.0f);
		// 壁のリストに追加
		mpWallObjects.push_back(wall);
	}
	// 下のタイルを確認
	if (!map[y + 1][x].passage && map[y + 1][x].type != CBspMap::TileType::eFloor)
	{
		// 壁の生成
		CWall* wall = new CWall(CVector(x  * TILE_SIZE, 0, y  * TILE_SIZE));
		// 壁の回転値を設定
		int rotY = ConvertDirectionAngle(CBspMap::Direction::eSouth);
		wall->Rotation(0.0f, rotY, 0.0f);
		// 壁のリストに追加
		mpWallObjects.push_back(wall);
	}
	// 左のタイルを確認
	if (!map[y][x - 1].passage && map[y][x - 1].type != CBspMap::TileType::eFloor)
	{
		// 壁の生成
		CWall* wall = new CWall(CVector(x  * TILE_SIZE, 0, y  * TILE_SIZE));
		// 壁の回転値を設定
		int rotY = ConvertDirectionAngle(CBspMap::Direction::eWest);
		wall->Rotation(0.0f, rotY, 0.0f);
		// 壁のリストに追加
		mpWallObjects.push_back(wall);
	}
}

// 通路の柱を生成
void CField::CreatePassagePillar(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y)
{
	// マップデータの外周は入れない
	if (y == 0 && y == map.size() - 1 && x == 0 && x == map[0].size() - 1) return;

	//方向に応じて座標を修正
	CVector pillarPos = CVector(x * TILE_SIZE, 0, y * TILE_SIZE);
	// オフセットポジション格納用
	CVector offSetPos = CVector::zero;

	// 北東(右上)に柱を生成するか
	if (map[y - 1][x].passage && map[y][x + 1].passage && !map[y - 1][x + 1].passage &&
		(map[y - 1][x + 1].type == CBspMap::TileType::None || map[y - 1][x + 1].type == CBspMap::TileType::eWall))
	{
		offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);

		// 柱の生成
		CPillar* pillar = new CPillar(pillarPos + offSetPos);
		// 柱のリストに追加
		mpPillarObjects.push_back(pillar);
	}
	// 南東(右下)に柱を生成するか
	if (map[y + 1][x].passage && map[y][x + 1].passage && !map[y + 1][x + 1].passage &&
		(map[y + 1][x + 1].type == CBspMap::TileType::None || map[y + 1][x + 1].type == CBspMap::TileType::eWall))
	{
		offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);

		// 柱の生成
		CPillar* pillar = new CPillar(pillarPos + offSetPos);
		// 柱のリストに追加
		mpPillarObjects.push_back(pillar);
	}
	// 北西(左上)に柱を生成するか
	if (map[y - 1][x].passage && map[y][x - 1].passage && !map[y - 1][x - 1].passage &&
		(map[y - 1][x - 1].type == CBspMap::TileType::None || map[y - 1][x - 1].type == CBspMap::TileType::eWall))
	{
		offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);

		// 柱の生成
		CPillar* pillar = new CPillar(pillarPos + offSetPos);
		// 柱のリストに追加
		mpPillarObjects.push_back(pillar);
	}
	// 南西(左下)に柱を生成するか
	if (map[y + 1][x].passage && map[y][x - 1].passage && !map[y + 1][x - 1].passage &&
		(map[y + 1][x - 1].type == CBspMap::TileType::None || map[y + 1][x - 1].type == CBspMap::TileType::eWall))
	{
		offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);

		// 柱の生成
		CPillar* pillar = new CPillar(pillarPos + offSetPos);
		// 柱のリストに追加
		mpPillarObjects.push_back(pillar);
	}
}

// 更新
void CField::Update()
{

#if _DEBUG
	if (CDebugInput::PushKey('B'))
	{
		CreateMap();
	}
#endif
}

// 描画
void CField::Render()
{
	//mpModel->Render(Matrix());

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
	// 通路の床の生成
	for (CFloor* passegeFloor : mpPassegeObjects)
	{
		passegeFloor->Render();
	}

}
