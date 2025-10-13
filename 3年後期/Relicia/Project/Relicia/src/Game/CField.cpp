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

#define PILLAR_OFFSET_POS 10.0f

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
				CFloor* floor = new CFloor(CVector(x * TILE_SIZE, 1, y * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
				// ���̃��X�g�ɒǉ�
				mpFloorObjects.push_back(floor);
			}
			// �ǂ̏ꍇ
			else if (tile.typeId == CDangeonMap::TileType::eWall)
			{
				// �ǂ̐���
				CWall* wall = new CWall(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));
				// �ǂ̉�]�l��ݒ�
				int rotY = ConvertDirectionAngle(tile.dir);
				wall->Rotation(0.0f, rotY, 0.0f);
				// �ǂ̃��X�g�ɒǉ�
				mpWallObjects.push_back(wall);
			}
			// ���̏ꍇ
			else if (tile.typeId == CDangeonMap::TileType::ePillar)
			{
				//�����ɉ����č��W���C��
				CVector pillarPos = CVector(x * TILE_SIZE, 0, y * TILE_SIZE);
				// �I�t�Z�b�g�|�W�V�����i�[�p
				CVector offSetPos;

				// �k���̏ꍇ
				if (tile.dir == CDangeonMap::Direction::eNorthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				// �쓌�̏ꍇ
				else if (tile.dir == CDangeonMap::Direction::eSouthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				// �쐼�̏ꍇ
				else if (tile.dir == CDangeonMap::Direction::eSouthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				//�k���̏ꍇ
				else if (tile.dir == CDangeonMap::Direction::eNorthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				pillarPos += offSetPos;

				// ���̐���
				CPillar* pillar = new CPillar(pillarPos);
				// ���̃��X�g�ɒǉ�
				mpPillarObjects.push_back(pillar);
			}
			// �o�����̏ꍇ
			else if (tile.typeId == CDangeonMap::TileType::eEntrance)
			{
				// �o�����̐���
				CEntrance* entrance = new CEntrance(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));
				int rotY = ConvertDirectionAngle(tile.dir);
				entrance->Rotation(0.0f, rotY, 0.0f);
				// �o�����̃��X�g�ɒǉ�
				mpEntranceObjects.push_back(entrance);

				CDoor* door = new CDoor(CVector(x * TILE_SIZE, 0, y * TILE_SIZE));
				door->Rotate(0.0f, rotY, 0.0f);
				// ���̃��X�g�ɒǉ�
				mpDoorObjects.push_back(door);
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
	// �o�����̕`��
	for (CEntrance* entrance : mpEntranceObjects)
	{
		entrance->Render();
	}
	// ���̕`��
	for (CPillar* pillar : mpPillarObjects)
	{
		pillar->Render();
	}
	// ���̕`��
	for (CDoor* door : mpDoorObjects)
	{
		door->Render();
	}

}
