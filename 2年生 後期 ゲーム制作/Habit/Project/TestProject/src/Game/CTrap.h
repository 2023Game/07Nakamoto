#pragma once
#include "CObjectBase.h"
#include "CModel.h"

// �G���ݒu�����
class CTrap : public CObjectBase
{
public:
	// �R���X�g���N�^
	CTrap();
	// �f�X�g���N�^
	~CTrap();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CModel* mpModel;		// 㩂̃��f���f�[�^
	CCollider* mpCollider;	// 㩂̃R���C�_�[
};