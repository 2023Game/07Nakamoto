#pragma once
#include "CCamera.h"

class CCat;

// �L�̃J����
class CCatCamera : public CCamera
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="cat">�n�_�ƂȂ�L�̃N���X</param>
	CCatCamera(CCat* cat);
	// �f�X�g���N�^
	~CCatCamera();

	// �X�V
	void Update() override;

private:
	CCat* mpCat;	// ���_�ƂȂ�L�̃N���X
	CVector mFollowDefaultEyeVec;
	CVector mRotateAngle;

};