#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CSwitch.h"
#include "CSlope.h"
#include "CMovingDoor.h"
#include "CUi.h"

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

	CApplication();

	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();

	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();

	static CUi* GetUi();	//UI�N���X�̃C���X�^���X���擾

private:

	//�v���C���[
	CPlayer mPlayer;
	//��
	CSlope mSlope;
	//�X�C�b�`
	//CSwitch mSwitch;
	//CSwitch mSwitch2;

	CSwitch mSwhith3;
	CMovingDoor mDoor;

	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	//�I�̃��f��
	CModel mModelTarget;
	//�}�b�v�̃��f��
	CModel mModelMap;
	//�w�i���f��
	CModel mBackGround;
	//��̃��f��
	CModel mModelSlope;
	//�X�C�b�`�̃��f��
	CModel mModelSwitch;

	//���l�p�`�̃��f��
	CModel mModelCube;

	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	CColliderMesh mColliderMesh2;
	//�⋭�R���C�_
	CColliderTriangle mTriangle;
	
	CInput mInput;

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;	//���_�A�����_�A�����
	CVector mEye;

	CCollider mC;

	EState mState;

	//���f���r���[�t�s��
	static CMatrix mModelViewInverse;

	static CUi* spUi;		//UI�N���X�̃|�C���^
};