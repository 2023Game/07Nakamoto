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

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	CModel* mpButtonModel;
	CCollider* mpButtonColMesh;

	bool mSwitch;
	// �����̍���
	CVector mDefaultPos;
	// �����ꂽ���̍��W
	CVector mOffSetPos;
};