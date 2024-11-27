#pragma once
#include "CObjectBase.h"
#include "CModel.h"

class CTorch : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="angle">��]</param>
	/// <param name="size">�傫��</param>
	CTorch(const CVector& pos, const CVector& angle, const CVector& size);

	// �f�X�g���N�^
	~CTorch();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// ���f���f�[�^


};