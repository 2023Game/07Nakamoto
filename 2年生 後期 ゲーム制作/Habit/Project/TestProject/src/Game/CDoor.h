#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// ���N���X
class CDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoor(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CDoor();

	// �ڑ�����X�C�b�`��ݒ�
	void SetSwitch(size_t index);

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// ���̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// ���̃R���W�����f�[�^

	//CPushSwitch* mpOwner;

};