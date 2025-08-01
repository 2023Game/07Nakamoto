#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

class CDemonWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDemonWall();
	// �f�X�g���N�^
	~CDemonWall();

	// �`��
	void Render() override;

private:
	CModel* mpModel;	// ���f���f�[�^
	CCollider* mpColliderMesh;	// �R���C�_�[
};