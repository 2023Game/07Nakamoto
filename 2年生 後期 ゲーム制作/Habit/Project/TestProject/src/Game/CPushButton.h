#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �{�^���̃N���X
class CPushButton : public CObjectBase
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CPushButton* Instance();
	// �R���X�g���N�^
	CPushButton(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CPushButton();

	// �v���C���[���{�^����������͈͂ɓ��������ǂ���
	bool isRangePlayer() const;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// �{�^���̃C���X�^���X
	static CPushButton* spInstance;

	CModel* mpModel;	// �{�^���̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// �{�^���̃R���W�����f�[�^

	bool mPush;	//�{�^����������Ă��邩
};