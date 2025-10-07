#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

#define ROOM_WIDTH 15
#define ROOM_HEIGHT 15

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// �����̑傫����ݒ�
	int mRoom[ROOM_HEIGHT][ROOM_WIDTH];

	// �^�C���̎��
	enum TileType {

		None = -1,

		eFloor,		// ��
		eWall,		// ��
		eEntrance,	// �o����
		eDoor,		// ��
		ePillar,	// ��
	};

	// �����̏�����
	void InitializeRoom();

	// �_���W�����̐���
	void CreateDungeon();

	// �����̃^�C���̃f�o�b�N�\��
	void PrintRoom();

};