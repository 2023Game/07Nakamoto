#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDungeonMap.h"
#include "CBpsMap.h"

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
	void CreateFieldObjects();

	// ���p�ɂ���ĉ�]�l��ݒ�
	int ConvertDirectionAngle(CDungeonMap::Direction dir) const;
	// ���p�ɂ���ĉ�]�l��ݒ�
	int ConvertDirectionAngle2(CBpsMap::Direction dir) const;

	// �����̐���
	void CreateRoom();
	// BPM�}�b�v�̕����̐���
	void SetMapData(const std::vector<std::vector<CBpsMap::Tile>>& map);

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// �Q�����z��̃}�b�v�f�[�^�ۑ��p
	CDungeonManeger* mpMapData;

	CBpsMap* mpMapData2;
	//std::vector<std::vector<CBpsMap::Tile>> mMap;

	// ���ꂼ���3D�I�u�W�F�N�g�̃��X�g
	std::vector<CFloor*> mpFloorObjects;	// ���̃��X�g
	std::vector<CWall*> mpWallObjects;		// �ǂ̃��X�g
	std::vector<CPillar*> mpPillarObjects;	// ���̃��X�g
	std::vector<CEntrance*> mpEntranceObjects;	// �o�����̃��X�g
	std::vector<CDoor*> mpDoorObjects;		// ���̃��X�g
};