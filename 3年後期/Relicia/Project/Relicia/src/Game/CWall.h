#pragma once
#include "CObjectBase.h"
#include "CCollider.h"
#include "CModel.h"

class CWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CWall(const CVector& pos);
	// �f�X�g���N�^
	~CWall();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	CModel* mpModel;
};