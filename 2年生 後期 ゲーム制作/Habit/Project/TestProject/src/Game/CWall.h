#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �Ǖ\���N���X
class CWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CWall(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CWall();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �ǂ̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// �ǂ̃R���W�����f�[�^
};