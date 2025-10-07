#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CEntrance : public CObjectBase
{
public:
	// �R���X�g���N�^
	CEntrance(const CVector& pos);
	// �f�X�g���N�^
	~CEntrance();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	CModel* mpModel;
};