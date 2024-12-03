#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// ���N���X
class CDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoor(CObjectBase* owner, const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CDoor();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// ���̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// ���̃R���W�����f�[�^

};