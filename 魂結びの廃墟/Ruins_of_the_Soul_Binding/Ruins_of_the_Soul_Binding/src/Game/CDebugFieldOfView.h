#pragma once
#include "CObjectBase.h"

// ����͈͂�\������f�o�b�O�@�\
class CDebugFieldOfView : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDebugFieldOfView(CObjectBase* owner,
		float fovAngle, float fovLength);
	// �f�X�g���N�^
	~CDebugFieldOfView();

	// ����͈͂�\������Ώۂ�ݒ�
	void SetOwner(CObjectBase* owner);
	// ����͈͂�ݒ�
	void Set(float angle, float length);
	// ����͈͂̍s���ݒ�
	void SetMatrix(CMatrix* mtx);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;	// �\������Ώۂ̃|�C���^
	CMatrix* mpMatrix;		// ����͈͂̍s��
	float mFovAngle;		// ����͈͂̊p�x(�f�B�O���[)	
	float mFovLength;		// ����͈͂̋���
};