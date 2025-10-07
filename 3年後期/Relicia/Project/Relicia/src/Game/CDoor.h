#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoor(CModel* model, const CVector& pos);
	// �f�X�g���N�^
	~CDoor();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpModel;

};