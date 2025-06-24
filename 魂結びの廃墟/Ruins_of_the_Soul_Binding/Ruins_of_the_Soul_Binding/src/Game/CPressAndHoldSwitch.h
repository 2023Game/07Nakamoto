#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CCollider.h"

class CPressAndHoldSwitch : public CObjectBase
{
	// �R���X�g���N�^
	CPressAndHoldSwitch(const CVector& pos);
	// �f�X�g���N�^
	~CPressAndHoldSwitch();

	// �{�^����������Ă��邩�ǂ���
	bool IsSwitchOn();

	// �{�^���̃I���I�t�؂�ւ�
	void ChangeSwith();

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

	CObjectBase* mpLastPushedObject;
	CObjectBase* mpPushedObject;

	bool mSwitch;
	// �����̍���
	CVector mDefaultPos;
	// �����ꂽ���̍��W
	CVector mOffSetPos;

	// �o�ߎ��Ԍv���p
	float mElapsedTime;
};