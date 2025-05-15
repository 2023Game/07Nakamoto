#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CRDoor();
	// �f�X�g���N�^
	~CRDoor();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

private:
	CModel* mpR_Door;
	CModel* mpR_DoorCol;

	CColliderMesh* mpR_DoorColliderMesh;
};