#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMoveWall.h"
#include "CColliderHitManager.h"
#include "CFloor.h"
#include "CObject.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//�v���C���[
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//�I
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//�e

//#define MODEL_MAP "res\\map4.obj","res\\map4.mtl"	//����}�b�v
//#define MODEL_MAP "res\\newmap3.obj","res\\newmap3.mtl"	//�}�b�v

#define MODEL_FLOOR	 "res\\floor.obj","res\\floor.mtl"	//��
#define MODEL_OBJECT "res\\newobject.obj","res\\newobject.mtl"	//�t�B�[���h�̃I�u�W�F�N�g

#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"		//�w�i��

//#define MODEL_SLOPE "res\\slopeobj.obj","res\\slopeobj.mtl"		//��(�R��)
#define MODEL_SLOPE "res\\slope.obj","res\\slope.mtl"		//��

#define MODEL_SPHERE "res\\sphere.obj" ,"res\\sphere.mtl"			//��(�X�C�b�`)

#define MODEL_MOVEWALL "res\\movecube.obj" ,"res\\movecube.mtl"			//������

#define MODEL_MOVEFLOOR "res\\movefloor.obj","res\\movefloor.mtl"	//������

void CApplication::Start()
{
	mState = EState::ESTART;
	mFlag = false;

	//�J�[�\���̍��W��ݒ肷��
	//SetCursorPos(400, 300);

	//���f���t�@�C���̓���
	CPlayer::GetModelPlayer()->Load(MODEL_TANK);
	CTarget::GetModelTarget()->Load(MODEL_TARGET);
	CFloor::GetModelFloor()->Load(MODEL_FLOOR);
	CObject::GetModelObject()->Load(MODEL_OBJECT);
	//mBackGround.Load(MODEL_SKY);
	CSlope::GetModelSlope()->Load(MODEL_SLOPE);
	CSwitch::GetModelSwitch()->Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMoveFloor::GetModelFloor()->Load(MODEL_MOVEFLOOR);
	CMoveWall::GetModelWall()->Load(MODEL_MOVEWALL);

	mpGame = new CGame();
}

void CApplication::Update()
{
	switch (mState)
	{
	case CApplication::EState::ESTART:
		//�X�^�[�g���
		mpGame->Start();
		
		if (mFlag == false)
		{
			//Enter�L�[�������ꂽ��
			if (mInput.Key(VK_RETURN))
			{
				//��Ԃ��v���C���ɂ���
				mState = EState::EPLAY;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		break;

	case CApplication::EState::EPLAY:
		//�Q�[����
		mpGame->Update();
		//�Q�[���I�[�o�[������
		if (mpGame->IsOver())
		{
			//��Ԃ��Q�[���I�[�o�[�ɂ���
			mState = EState::EOVER;
		}
		//�Q�[���N���A�����肷��
		if (mpGame->IsClear())
		{
			//��Ԃ��Q�[���N���A�ɂ���
			mState = EState::ECLEAR;
		}
		break;

	case CApplication::EState::ECLEAR:
		//�Q�[���N���A���
		mpGame->Clear();

		if (mFlag == false)
		{
			//Enter�L�[�������ꂽ��
			if (mInput.Key(VK_RETURN))
			{
				//�Q�[���̃C���X�^���X�폜
				delete mpGame;
				//�Q�[���̃C���X�^���X����
				mpGame = new CGame();
				//��Ԃ��X�^�[�g�ɂ���
				mState = EState::ESTART;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		break;

	case CApplication::EState::EOVER:
		//�Q�[���I�[�o�[����
		mpGame->Over();
		if (mFlag == false)
		{
			//Enter�L�[�������ꂽ��
			if (mInput.Key(VK_RETURN))
			{
				//�Q�[���̃C���X�^���X�폜
				delete mpGame;
				//�Q�[���̃C���X�^���X����
				mpGame = new CGame();
				//��Ԃ��X�^�[�g�ɂ���
				mState = EState::ESTART;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		
		break;

	//case CApplication::EState::ERSTART:
	//	break;
	//default:
	//	break;

	}
}

CMatrix CApplication::mModelViewInverse;
//���f���r���[�s��̎擾
const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;
//UI�N���X�̃C���X�^���X���擾
CUi* CApplication::GetUi()
{
	return spUi;	//�C���X�^���X�̃|�C���^��Ԃ�
}

