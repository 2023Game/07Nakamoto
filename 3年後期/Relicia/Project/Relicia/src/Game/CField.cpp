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

#include "CDebugInput.h"

#define PILLAR_OFFSET_POS 10.0f	// ���̃I�t�Z�b�g���W

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
	, mpMapData(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	//CreateFieldObjects();

	// BSP�@�Ń_���W��������
	CreateMap();
}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}

	SAFE_DELETE(mpMapData);
}

void CField::CreateMap()
{
	if (mpMapData != nullptr)
	{
		SAFE_DELETE(mpMapData);
		for (CFloor* floor : mpFloorObjects) floor->Kill();
		for (CWall* wall : mpWallObjects) wall->Kill();
		for (CPillar* cpillar : mpPillarObjects) cpillar->Kill();
		for (CEntrance* entrance : mpEntranceObjects) entrance->Kill();
		for (CDoor* door : mpDoorObjects) door->Kill();

		mpFloorObjects.clear();
		mpWallObjects.clear();
		mpPillarObjects.clear();
		mpEntranceObjects.clear();
		mpDoorObjects.clear();
	}
	// BSP�@�Ń_���W��������
	mpMapData = new CBspMap(50, 50);
	SetMapData(mpMapData->GetTileData());
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
int CField::ConvertDirectionAngle(CBspMap::Direction dir) const
{
	switch (dir)
	{
		// �k�̏ꍇ
	case CBspMap::Direction::eSouth:	return 0;
		// ���̏ꍇ
	case CBspMap::Direction::eWest:		return 90;
		// ��̏ꍇ
	case CBspMap::Direction::eNorth:	return 180;
		// ���̏ꍇ
	case CBspMap::Direction::eEast:		return 270;

	default:	return 0;
	}
}

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
				// ���̐���
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
				// ���̃��X�g�ɒǉ�
				mpFloorObjects.push_back(floor);

				break;
			}
			case CBspMap::TileType::eWall:
			{
				// �ǂ̐���
				CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				// �ǂ̉�]�l��ݒ�
				int rotY = ConvertDirectionAngle(tile.dir);
				wall->Rotation(0.0f, rotY, 0.0f);
				// �ǂ̃��X�g�ɒǉ�
				mpWallObjects.push_back(wall);

				// �ʘH��������A������������
				if (map[y][x].passage)
				{
					// ���̐���
					CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
					// ���̃��X�g�ɒǉ�
					mpFloorObjects.push_back(floor);
				}
				// �ʘH�������琳���΂ɂ��ǂ𐶐�����
				if (map[y][x].wall)
				{
					// �ǂ̐���
					CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
					// �ǂ̉�]�l��ݒ�
					switch (tile.dir)
					{
					case CBspMap::Direction::eNorth: wall->Rotation(CVector(0.0f, 0.0f, 0.0f));		break;
					case CBspMap::Direction::eEast: wall->Rotation(CVector(0.0f, 90.0f, 0.0f));		break;
					case CBspMap::Direction::eSouth: wall->Rotation(CVector(0.0f, 180.0f, 0.0f));	break;
					case CBspMap::Direction::eWest: wall->Rotation(CVector(0.0f, 270.0f, 0.0f));	break;
					default:
						break;
					}
					// �ǂ̃��X�g�ɒǉ�
					mpWallObjects.push_back(wall);
				}

				break;
			}
			case CBspMap::TileType::ePillar:
			{
				//�����ɉ����č��W���C��
				CVector pillarPos = CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE);
				// �I�t�Z�b�g�|�W�V�����i�[�p
				CVector offSetPos;

				// �k���̏ꍇ
				if (tile.dir == CBspMap::Direction::eNorthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				// �쓌�̏ꍇ
				else if (tile.dir == CBspMap::Direction::eSouthEast)
				{
					offSetPos = CVector(-PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				// �쐼�̏ꍇ
				else if (tile.dir == CBspMap::Direction::eSouthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, -PILLAR_OFFSET_POS);
				}
				//�k���̏ꍇ
				else if (tile.dir == CBspMap::Direction::eNorthWest)
				{
					offSetPos = CVector(PILLAR_OFFSET_POS, 0.0f, PILLAR_OFFSET_POS);
				}
				pillarPos += offSetPos;

				// ���̐���
				CPillar* pillar = new CPillar(pillarPos);
				// ���̃��X�g�ɒǉ�
				mpPillarObjects.push_back(pillar);

				// �ʘH��������A������������
				if (map[y][x].passage)
				{
					// ���̐���
					CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
					// ���̃��X�g�ɒǉ�
					mpFloorObjects.push_back(floor);
				}
				break;
			}
			case CBspMap::TileType::eEntrance:
			{
				// �o�����̐���
				CEntrance* entrance = new CEntrance(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
				int rotY = ConvertDirectionAngle(tile.dir);
				entrance->Rotation(0.0f, rotY, 0.0f);
				// �o�����̃��X�g�ɒǉ�
				mpEntranceObjects.push_back(entrance);

				// ���̐���
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
				// ���̃��X�g�ɒǉ�
				mpFloorObjects.push_back(floor);

				//CDoor* door = new CDoor(CVector((x + 0.5f) * TILE_SIZE + offSetPosX, 0, (y + 0.5f) * TILE_SIZE + offSetPosZ));
				//door->Rotate(0.0f, rotY, 0.0f);
				//// ���̃��X�g�ɒǉ�
				//mpDoorObjects.push_back(door);
				break;
			}
			case CBspMap::TileType::ePassage:
			{
				// ���̐���
				CFloor* floor = new CFloor(CVector((x + 0.5f) * TILE_SIZE, 1, (y + 0.5f) * TILE_SIZE));	// �R���C�_�[���o������Y���W��0�ɕύX
				// ���̃��X�g�ɒǉ�
				mpFloorObjects.push_back(floor);

				// �k�����̏ꍇ
				if (map[y][x].dir == CBspMap::Direction::eNorth && map[y][x].wall)
				{
					// �E�̕ǂ̐���
					CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
					// �ǂ̉�]�l��ݒ�
					wall->Rotation(0.0f, 90.0f, 0.0f);
					// �ǂ̃��X�g�ɒǉ�
					mpWallObjects.push_back(wall);
					// ���̕ǂ̐���
					wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
					// �ǂ̉�]�l��ݒ�
					wall->Rotation(0.0f, 270.0f, 0.0f);
					// �ǂ̃��X�g�ɒǉ�
					mpWallObjects.push_back(wall);
				}
				// �������̏ꍇ
				else if (map[y][x].dir == CBspMap::Direction::eEast && map[y][x].wall)
				{
					// ��̕ǂ̐���
					CWall* wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
					// �ǂ̉�]�l��ݒ�
					wall->Rotation(0.0f, 180.0f, 0.0f);
					// �ǂ̃��X�g�ɒǉ�
					mpWallObjects.push_back(wall);
					// ���̕ǂ̐���
					wall = new CWall(CVector((x + 0.5f) * TILE_SIZE, 0, (y + 0.5f) * TILE_SIZE));
					// �ǂ̉�]�l��ݒ�
					wall->Rotation(0.0f, 0.0f, 0.0f);
					// �ǂ̃��X�g�ɒǉ�
					mpWallObjects.push_back(wall);

				}
				break;
			}
			default:
				break;
			}
		}
	}
}

// �X�V
void CField::Update()
{

#if _DEBUG
	if (CDebugInput::PushKey('B'))
	{
		CreateMap();
	}
#endif
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
