#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

// �X�C�b�`�̓y��N���X
class CStand : public CObjectBase
{
public:
	//�R���X�g���N�^
	CStand(const CVector& pos);
	// �f�X�g���N�^
	~CStand();

	// �`��
	void Render();

private:
	CModel* mpStandModel;
	CCollider* mpStandWallColMesh;	// �y��̕Ǖ����̃R���C�_�[
	CCollider* mpStandFloorColMesh;	// �y��̏������̃R���C�_�[
};