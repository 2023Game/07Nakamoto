#pragma once
#include "CCamera.h"

// �Q�[�����̃J����
class CPlayerCamera : public CCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="eye">�J�����̎��_</param>
	/// <param name="center">�J�����̒����_</param>
	/// <param name="isMainCamera">���C���J�������ǂ���</param>
	CPlayerCamera(const CVector& eye, const CVector& center, bool isMainCamera = true);
	// �f�X�g���N�^
	~CPlayerCamera();

	void SetFollowTargetTf(CTransform* target) override;
	void LookAt(const CVector& eye, const CVector& at,
		const CVector& up, bool updateTargetEye = true) override;

	// �X�V
	void Update() override;

private:
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;
};