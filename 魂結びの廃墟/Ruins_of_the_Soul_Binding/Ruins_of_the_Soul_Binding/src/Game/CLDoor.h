#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CLDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CLDoor();
	// �f�X�g���N�^
	~CLDoor();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

private:
	CModel* mpL_Door;
	CModel* mpL_DoorCol;

	CColliderMesh* mpL_DoorColliderMesh;

};