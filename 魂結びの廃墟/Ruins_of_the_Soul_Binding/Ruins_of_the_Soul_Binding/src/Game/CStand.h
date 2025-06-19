#pragma once
#include "CObjectBase.h"

class CModel;
class CCollider;

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
	CCollider* mpStandColMesh;

};