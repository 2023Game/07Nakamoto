#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CInteractObject.h"

class CCollider;
class CBillBoard;

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

	// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
	std::string GetInteractTextPath() const override;
	// ���ׂ�UI��\��������W��Ԃ�
	virtual CVector GetInteractUIPos() const override;

	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �X�C�b�`�̃��f���f�[�^
	CCollider* mpCollider;	// �X�C�b�`�̃R���W�����f�[�^

	bool mSwitch;	// �X�C�b�`���I�����I�t��
};