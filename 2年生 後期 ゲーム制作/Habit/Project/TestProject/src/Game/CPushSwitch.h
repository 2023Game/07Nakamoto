#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �X�C�b�`�̃N���X
class CPushSwitch : public CObjectBase
{
public:
	// �R���X�g���N�^
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CPushSwitch();


	
	// �X�C�b�`�������ꂽ���ǂ���
	bool IsOnSwtch();

	// �ԍ��̎擾
	int GetNumber();

	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �X�C�b�`�̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// �X�C�b�`�̃R���W�����f�[�^

	bool mSwitch;	// �X�C�b�`���I�����I�t��
	int mNumber;	// �ԍ�
	int mNum;		// �����ꂽ��

};