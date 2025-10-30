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
class CDungeonManeger;

class CField : public CObjectBase
{
public:
	// �R���X�g���N�^
	CField();
	// �f�X�g���N�^
	~CField();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �_���W�����}�b�v�̐���
	void CreateMap();

	void CreateFieldObjects();

	// ���p�ɂ���ĉ�]�l��ݒ�
	int ConvertDirectionAngle(CBspMap::Direction dir) const;
	// BPM�}�b�v�̃_���W�����̐���
	void SetMapData(const std::vector<std::vector<CBspMap::Tile>>& map);

	// �ʘH�̕ǂ̐���
	void CreatePassageWall(const std::vector<std::vector<CBspMap::Tile>>& map, int x, int y);

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
	
	// �Q�����z��̃}�b�v�f�[�^�ۑ��p
	CBspMap* mpMapData;

	// ���ꂼ���3D�I�u�W�F�N�g�̃��X�g
	std::vector<CFloor*> mpFloorObjects;	// ���̃��X�g
	std::vector<CWall*> mpWallObjects;		// �ǂ̃��X�g
	std::vector<CPillar*> mpPillarObjects;	// ���̃��X�g
	std::vector<CEntrance*> mpEntranceObjects;	// �o�����̃��X�g
	std::vector<CDoor*> mpDoorObjects;		// ���̃��X�g
};