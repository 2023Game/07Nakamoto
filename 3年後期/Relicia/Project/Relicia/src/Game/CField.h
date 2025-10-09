#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CDangeonMap.h"

class CFloor;
class CWall;
class CPillar;
class CDoor;
class Direction;

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
	int ConvertDirectionAngle(CDangeonMap::Direction dir) const;

	// �����̐���
	void CreateRoom();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// �Q�����z��̃}�b�v�f�[�^�ۑ��p
	CDangeonMap* mpMapData;

	// ���ꂼ���3D�I�u�W�F�N�g�̃��X�g
	std::vector<CFloor*> mpFloorObjects;	// ���̃��X�g
	std::vector<CWall*> mpWallObjects;		// �ǂ̃��X�g
	std::vector<CPillar*> mpCPillarObjects;	// ���̃��X�g
	std::vector<CDoor*> mpDoorObjects;		// ���̃��X�g
};