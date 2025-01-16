#pragma once
#include "CObjectBase.h"
#include "CModel.h"

class CCollider;

// �`���R���[�g�̃N���X
class CChoco : public CObjectBase
{
public:
	// �R���X�g���N�^
	CChoco();
	// �f�X�g���N�^
	~CChoco();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;		// �`���R�̃��f���f�[�^
	CCollider* mpCollider;	// �`���R�̃R���C�_�[
};