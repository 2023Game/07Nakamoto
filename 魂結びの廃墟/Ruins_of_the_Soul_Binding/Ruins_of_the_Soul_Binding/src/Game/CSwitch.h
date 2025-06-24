#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CCollider.h"

class CSwitch : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitch(const CVector& pos);
	// �f�X�g���N�^
	~CSwitch();

	// �{�^����������Ă��邩�ǂ���
	bool IsSwitchOn();

	// �{�^���̃I���I�t�؂�ւ�
	void ChangeSwith();

	// ���݃{�^���������Ă���I�u�W�F�N�g���ǂ���
	bool IsPushedObject(CObjectBase* obj) const;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CModel* mpButtonModel;
	CCollider* mpButtonColMesh;

	// �X�C�b�`�̏�ɏ���Ă���I�u�W�F�N�g�̃|�C���^
	CObjectBase* mpPushedObject;

	bool mSwitch;
	// �����̍���
	CVector mDefaultPos;
	// �����ꂽ���̍��W
	CVector mOffSetPos;

	// �o�ߎ��Ԍv���p
	float mElapsedTime;
};