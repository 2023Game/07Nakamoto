#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CDangeonMap.h"
#include "CFloor.h"
#include "CDoor.h"
#include "CWall.h"
#include "CPillar.h"
#include "CEntrance.h"

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	//CreateFieldObjects();

	mpMapData = new CDangeonMap();

	CreateRoom();
}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
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
int CField::ConvertDirectionAngle(CDangeonMap::Direction dir) const
{
	switch (dir)
	{
		// 北の場合
		case CDangeonMap::Direction::eNorth:	return 0;
		// 東の場合
		case CDangeonMap::Direction::eEast:		return 90;
		// 南の場合
		case CDangeonMap::Direction::eSouth:	return 180;
		// 西の場合
		case CDangeonMap::Direction::eWest:		return 270;

		default:	return 0;
	}
}

// 部屋の生成
void CField::CreateRoom()
{
	// 配列データの列
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		// 配列データの行
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			// タイル情報の2次元配列のデータを取得
			CDangeonMap::Tile tile = mpMapData->GetTile(x, y);

			// 床の場合
			if (tile.typeId == CDangeonMap::TileType::eFloor)
			{
				// 床の生成
				CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 1, y * TILE_SIZE));
				mpFloorObjects.push_back(floor);
			}
			// 壁の場合
			else if (tile.typeId == CDangeonMap::TileType::eWall)
			{
				// 壁の生成
				CWall* wall = new CWall(CVector(x * TILE_SIZE, 1, y * TILE_SIZE));
				// 壁の回転値を設定
				int rotY = ConvertDirectionAngle(tile.dir);
				wall->Rotation(0.0f, rotY, 0.0f);

				mpWallObjects.push_back(wall);
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
}
