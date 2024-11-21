#pragma once
#include "CObjectBase.h"

// �G���ݒu�����
class CTrap : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pwner">㩂𐶐������I�u�W�F�N�g</param>
	/// <param name="pos">�����ʒu</param>
	CTrap(const CVector& pos);
	// �f�X�g���N�^
	~CTrap();

	// �Օ����`�F�b�N��\���p�ɁA������ɏグ���m�[�h�̍��W��Ԃ�
	CVector GetOffsetPos() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CVector mPosition;
	CColor mColor;
};