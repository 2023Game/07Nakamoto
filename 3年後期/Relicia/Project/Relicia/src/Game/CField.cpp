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

	// �����Ȃ����@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// �������@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// ��]���鏰�@
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// �����Ȃ����A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// �����Ȃ����B�i�⓹�j
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// �d���G�t�F�N�g
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

// ���p�ɂ���ĉ�]�l��ݒ�
int CField::ConvertDirectionAngle(CDangeonMap::Direction dir) const
{
	switch (dir)
	{
		// �k�̏ꍇ
		case CDangeonMap::Direction::eNorth:	return 0;
		// ���̏ꍇ
		case CDangeonMap::Direction::eEast:		return 90;
		// ��̏ꍇ
		case CDangeonMap::Direction::eSouth:	return 180;
		// ���̏ꍇ
		case CDangeonMap::Direction::eWest:		return 270;

		default:	return 0;
	}
}

// �����̐���
void CField::CreateRoom()
{
	// �z��f�[�^�̗�
	for (int y = 0; y < ROOM_HEIGHT; y++)
	{
		// �z��f�[�^�̍s
		for (int x = 0; x < ROOM_WIDTH; x++)
		{
			// �^�C������2�����z��̃f�[�^���擾
			CDangeonMap::Tile tile = mpMapData->GetTile(x, y);

			// ���̏ꍇ
			if (tile.typeId == CDangeonMap::TileType::eFloor)
			{
				// ���̐���
				CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 1, y * TILE_SIZE));
				mpFloorObjects.push_back(floor);
			}
			// �ǂ̏ꍇ
			else if (tile.typeId == CDangeonMap::TileType::eWall)
			{
				// �ǂ̐���
				CWall* wall = new CWall(CVector(x * TILE_SIZE, 1, y * TILE_SIZE));
				// �ǂ̉�]�l��ݒ�
				int rotY = ConvertDirectionAngle(tile.dir);
				wall->Rotation(0.0f, rotY, 0.0f);

				mpWallObjects.push_back(wall);
			}

		}
	}

}

// �X�V
void CField::Update()
{
}

// �`��
void CField::Render()
{
	mpModel->Render(Matrix());

	// ���̕`��
	for (CFloor* floor : mpFloorObjects)
	{
		floor->Render();
	}
	// �ǂ̕`��
	for (CWall* wall : mpWallObjects)
	{
		wall->Render();
	}
}
