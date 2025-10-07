#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CPillar : public CObjectBase
{
public:
	// �R���X�g���N�^
	CPillar(const CVector& pos);
	// �f�X�g���N�^
	~CPillar();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	CModel* mpModel;
};