#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CSwitch.h"
#include "CSlope.h"
#include "CMoveWall.h"
#include "CUi.h"
#include "CGame.h"

class CApplication
{
public:
	enum class EState
	{
		ESTART,		//�Q�[���J�n
		EPLAY,		//�Q�[����
		ECLEAR,		//�Q�[���N���A
		EOVER,		//�Q�[���I�[�o�[
		ERSTART,	//�ăX�^�[�g
	};

	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();

	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	//UI�N���X�̃C���X�^���X���擾
	static CUi* GetUi();	

private:
	//�Q�[��
	CGame* mpGame;

	//���f���N���X�̃C���X�^���X�쐬
	//�v���C���[�̃��f��
	CModel mModel;
	//�I�̃��f��
	CModel mModelTarget;
	//��
	CModel mFloor;
	//�I�u�W�F�N�g
	CModel mObject;
	//�w�i���f��
	//CModel mBackGround;
	//��̃��f��
	CModel mModelSlope;
	//�X�C�b�`�̃��f��
	CModel mModelSwitch;
	
	CInput mInput;

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;	//���_�A�����_�A�����

	EState mState;

	//���f���r���[�t�s��
	static CMatrix mModelViewInverse;

	static CUi* spUi;	//UI�N���X�̃|�C���^

	bool mFlag;
};