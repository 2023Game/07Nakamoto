#pragma once
#include "CModel.h"
#include "CInput.h"

class CApplication
{
public:
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();

private:
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	//�w�i���f��
	CModel mBackGround;

	CInput mInput;
	CVector mEye;
};