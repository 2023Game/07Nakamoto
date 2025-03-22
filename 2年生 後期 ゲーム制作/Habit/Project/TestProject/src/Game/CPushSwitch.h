#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;
class CBillBoard;
class CInteractUI;

// �X�C�b�`�̃N���X
class CPushSwitch : public CInteractObject
{
public:
	// �R���X�g���N�^
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CPushSwitch();
	
	// �X�C�b�`�������ꂽ���ǂ���
	bool IsOnSwtch();

	// ���ׂ�
	void Interact() override;

	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �X�C�b�`�̃��f���f�[�^
	CCollider* mpCollider;	// �X�C�b�`�̃R���W�����f�[�^
	CInteractUI* mpInteractUI;	// �C���^���N�g�{�^���̃C���[�W

	bool mSwitch;	// �X�C�b�`���I�����I�t��
};