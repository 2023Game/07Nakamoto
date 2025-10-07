#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoor(const CVector& pos);
	// �f�X�g���N�^
	~CDoor();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CModel* mpModel;
	
};