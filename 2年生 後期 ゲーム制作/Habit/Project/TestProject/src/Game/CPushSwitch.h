#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;

// �X�C�b�`�̃N���X
class CPushSwitch : public CInteractObject
{
public:
	// �R���X�g���N�^
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CPushSwitch();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;
	
	// �X�C�b�`�������ꂽ���ǂ���
	bool IsOnSwtch();

	// �ԍ��̎擾
	//int GetNumber();

	// ���ׂ�
	void Interact() override;

	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �X�C�b�`�̃��f���f�[�^
	CCollider* mpCollider;	// �X�C�b�`�̃R���W�����f�[�^

	bool mSwitch;	// �X�C�b�`���I�����I�t��
	int mNumber;	// �ԍ�
	int mNum;		// �����ꂽ��

};