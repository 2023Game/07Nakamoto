#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �N���A�G���A�@�N���X
class CClearArea : public CObjectBase
{
public:
	// �R���X�g���N�^
	CClearArea(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CClearArea();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	//// �X�V����
	//void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �N���A�G���A�̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// �N���A�G���A�̃R���W�����f�[�^
};