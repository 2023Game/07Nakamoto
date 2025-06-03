#pragma once
#include "CCamera.h"

// �L�̃J����
class CCatCamera : public CCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="eye">�J�����̎��_</param>
	/// <param name="center">�J�����̒����_</param>
	/// <param name="isMainCamera">���C���J�������ǂ���</param>
	CCatCamera(const CVector& eye, const CVector& center, bool isMainCamera = false);
	// �f�X�g���N�^
	~CCatCamera();

	/// <summary>
	/// �Ǐ]����^�[�Q�b�g��ݒ�
	/// </summary>
	/// <param name="target">�Ǐ]����^�[�Q�b�g</param>
	void SetFollowTargetTf(CTransform* target) override;

	/// <summary>
	/// ��������ʒu��ݒ�i���_ + �����_ + ��x�N�g���j
	/// </summary>
	/// <param name="eye">�J�����̈ʒu</param>
	/// <param name="at">��������ʒu</param>
	/// <param name="up">��x�N�g��</param>
	/// <param name="updateTargetEye">���_�̖ڕW�ʒu���X�V���邩�ǂ���</param>
	void LookAt(const CVector& eye, const CVector& at,
		const CVector& up, bool updateTargetEye = true) override;

	// �X�V
	void Update() override;

private:
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;

};