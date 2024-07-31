#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMoveWall.h"
#include "CColliderHitManager.h"

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

	//�J�[�\���̍��W��ݒ肷��
	//SetCursorPos(400, 300);

	//���f���t�@�C���̓���
	//mModel.Load(MODEL_TANK);
	CPlayer::GetModelPlayer()->Load(MODEL_TANK);
	//mModelTarget.Load(MODEL_TARGET);
	CTarget::GetModelTarget()->Load(MODEL_TARGET);
	//mFloor.Load(MODEL_FLOOR);
	CColliderMesh::GetModelFloor()->Load(MODEL_FLOOR);
	//mObject.Load(MODEL_OBJECT);
	CColliderMesh::GetModelObject()->Load(MODEL_OBJECT);
	mBackGround.Load(MODEL_SKY);
	//mModelSlope.Load(MODEL_SLOPE);
	CSlope::GetModelSlope()->Load(MODEL_SLOPE);
	//mModelSwitch.Load(MODEL_SPHERE);
	CSwitch::GetModelSwitch()->Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMoveFloor::GetModelFloor()->Load(MODEL_MOVEFLOOR);
	CMoveWall::GetModelWall()->Load(MODEL_MOVEWALL);

	mpGame = new CGame();

	/*
	//�v���C���[����
	mPlayer.SetModel(&mModel);
	mPlayer.SetScale(CVector(1.5f, 1.5f, 1.5f));
	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));

	//�I�̃R���C�_�𐶐�
	new CTarget(&mModelTarget, CVector(-21.0f, 4.5f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(85.0f, 24.0f, 35.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(1.5f, 10.0f, 33.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(30.0f, 40.0f, -100.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(60.0f, 9.0f, 5.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	//�X�C�b�`�̐���
	mSwhith.SetSwitch(&mModelSwitch,
		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//�����ǂ̐���
	mMoveWall.SetMoveWall(&mSwhith, 
		CVector(-19.0f, 5.0f, -1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//������
	mMoveFloor.SetMoveFloor(CVector(31.0f, 12.0f, -35.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//��̐���
	mSlope.SetSlope(CVector(-16.5f, 5.325f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.065f, 0.8f), &mModelSlope);
	mSlope2.SetSlope(CVector(31.0f, 4.25f, -21.25f), CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 0.875f, 0.8f), &mModelSlope);

	//���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X
	// �Ɛe�s��͖���
	//�w�i�̃��f��
	mColliderMesh4.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	//�X�e�[�W�̃��f��
	mCSlope.ColliderMeshSet(&mSlope, mSlope.GetMatrix(), &mModelSlope,CCollider::ETag::ESLOPE);		//��1
	mCSlope2.ColliderMeshSet(&mSlope2, mSlope2.GetMatrix(), &mModelSlope, CCollider::ETag::ESLOPE); //��2
	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mFloor,CCollider::ETag::EFLOOR);				//��
	mColliderMesh3.ColliderMeshSet(nullptr, nullptr, &mObject);										//�I�u�W�F�N�g
	

	//�r���{�[�h�̐���
	//new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);

	//UI�N���X�̐���
	spUi = new CUi();

	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();
	*/
}

void CApplication::Update()
{
	switch (mState)
	{
	case CApplication::EState::ESTART:
		//�X�^�[�g���
		mpGame->Start();
		//Enter�L�[�������ꂽ��
		if (mInput.Key(VK_RETURN))
		{
			//��Ԃ��v���C���ɂ���
			mState = EState::EPLAY;
		}
		break;



		//���_�����߂�
		e = CVector(60.0f, 2.0f, 10.0f);
		//�����_�����߂�
		c = CVector(0.0f,0.0f,0.0f);
		//����������߂�
		u = CVector(0, 1, 0);
		//�J�����ݒ�
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//���f���r���[�s��̎擾
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//�t�s��̎擾
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//�w�i
		mBackGround.Render();

		//�}�b�v�̕`��
		mFloor.Render();
		mObject.Render();

		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::GetInstance()->Render();

		spUi->State();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			mState = EState::EPLAY;
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



		//�^�X�N�}�l�[�W���̍X�V
		CTaskManager::GetInstance()->Update();
		//�R���W�����}�l�[�W���̏Փ˔���
		CTaskManager::GetInstance()->Collision();

	//	mPlayer.HitCollision(mPlayer.GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

		CColliderHitManager::GetInstance()->Delete();

		//���_�����߂�
	//	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//�����_�����߂�
	//	c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//����������߂�
	//	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
		//�J�����ݒ�
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//���f���r���[�s��̎擾
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//�t�s��̎擾
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//�w�i
		mBackGround.Render();

		//�}�b�v�̕`��
		mFloor.Render();
		mObject.Render();

		//�^�X�N���X�g�̍폜
		CTaskManager::GetInstance()->Delete();
		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::GetInstance()->Render();
		//�R���W�����}�l�[�W���̕`��
		CCollisionManager::GetInstance()->Render();

		spUi->Time();
		spUi->Render();

		if (CUi::GetPoint() == 5)
		{
			mState = EState::ECLEAR;		
		}
		break;
		
	case CApplication::EState::ECLEAR:
		//�Q�[���N���A���
		mpGame->Clear();
		//Enter�L�[�������ꂽ��
		if (mInput.Key(VK_RETURN))
		{
			//�Q�[���̃C���X�^���X�폜
			delete mpGame;
			//�Q�[���̃C���X�^���X����
			mpGame = new CGame();
			//��Ԃ��X�^�[�g�ɂ���
			mState = EState::ESTART;
		}
		break;



		//���_�����߂�
	//	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//�����_�����߂�
	//	c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//����������߂�
	//	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
		//�J�����ݒ�
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//���f���r���[�s��̎擾
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//�t�s��̎擾
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//�w�i
		mBackGround.Render();

		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::GetInstance()->Render();

		spUi->Clear();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			CTaskManager::GetInstance()->AllDelete();

			CUi::SetPoint();
			mState = EState::ERSTART;
		}

		break;

	case CApplication::EState::EOVER:
		//�Q�[���I�[�o�[����
		mpGame->Over();
		//Enter�L�[�������ꂽ��
		if (mInput.Key(VK_RETURN))
		{
			//�Q�[���̃C���X�^���X�폜
			delete mpGame;
			//�Q�[���̃C���X�^���X����
			mpGame = new CGame();
			//��Ԃ��X�^�[�g�ɂ���
			mState = EState::ESTART;
		}
		break;

	case CApplication::EState::ERSTART:

		//�v���C���[�̐ݒ�
	//	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	//	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));

		//�I�̃R���C�_�𐶐�
		new CTarget(&mModelTarget, CVector(-21.0f, 4.5f, -1.0f),
			CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(85.0f, 24.0f, 35.0f),
			CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(1.5f, 10.0f, 33.0f),
			CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(30.0f, 40.0f, -100.0f),
			CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(60.0f, 9.0f, 5.0f),
			CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

		//�X�C�b�`�̐���
	//	mSwhith.SetSwitch(&mModelSwitch,
	//		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
		//�����ǂ̐���
	//	mMoveWall.SetMoveWall(&mSwhith,
	//		CVector(-19.0f, 5.0f, -1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

		//������
	//	mMoveFloor.SetMoveFloor(CVector(31.0f, 12.0f, -35.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
		
		mState = EState::EPLAY;

		break;
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

