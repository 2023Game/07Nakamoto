#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CFloor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CFloor(const CVector& pos);
	// �f�X�g���N�^
	~CFloor();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	CModel* mpModel;
};