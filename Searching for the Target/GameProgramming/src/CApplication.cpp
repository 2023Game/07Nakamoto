#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMovingDoor.h"

#include "CColliderHitManager.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//�v���C���[
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//�I
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//�e

//#define MODEL_MAP "res\\map4.obj","res\\map4.mtl"	//����}�b�v
//#define MODEL_MAP "res\\newmap3.obj","res\\newmap3.mtl"	//�}�b�v

#define MODEL_FLOOR	 "res\\newfloor.obj","res\\newfloor.mtl"	//��
#define MODEL_OBJECT "res\\newobject.obj","res\\newobject.mtl"	//�I�u�W�F�N�g

#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"		//�w�i��

//#define MODEL_SLOPE "res\\slopeobj.obj","res\\slopeobj.mtl"		//��
#define MODEL_SLOPE "res\\newslope1.obj","res\\newslope1.mtl"		//��

#define MODEL_SPHERE "res\\sphere.obj" ,"res\\sphere.mtl"		//��(�X�C�b�`)

#define MODEL_BLUECUBE "res\\movecube.obj" ,"res\\movecube.mtl"	//�F�̎l�p�`

CApplication::CApplication()
	: mState(EState::EPLAY)
{

}

void CApplication::Start()
{
	//�J�[�\���̍��W��ݒ肷��
	//SetCursorPos(400, 300);

	//���f���t�@�C���̓���
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	//mModelMap.Load(MODEL_MAP);

	mFloor.Load(MODEL_FLOOR);
	mObject.Load(MODEL_OBJECT);

	//mBackGround.Load(MODEL_SKY);
	mModelSlope.Load(MODEL_SLOPE);
	mModelSwitch.Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMovingDoor::GetModelCube()->Load(MODEL_BLUECUBE);

	mEye = CVector(0.0f, 2.0f, 13.0f);

	CMatrix matrix;
	matrix.Print();

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
	mSwhith3.SetSwitch(&mModelSwitch,
		CVector(-15.0f, 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//�����ǂ̐���
	mDoor.SetMovingDoor(&mSwhith3, 
		CVector(-18.5f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//��̐���
	mSlope.SetSlope(CVector(-16.0f, 0.0f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f), &mModelSlope);

	//���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X
	// �Ɛe�s��͖���
	//�w�i�̃��f��
	//mColliderMesh.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	//�����Ă��܂��Ƃ����⋭����R���C�_
	//mTriangle.SetColliderTriangle(nullptr, nullptr, CVector(-13.0f, -1.0f, 4.0f), CVector(13.0f, -1.0f, 4.0f), CVector(0.0f, -1.0f, -13.0f));

	//�X�e�[�W�̃��f��
	mColliderMesh.ColliderMeshSet(&mSlope, mSlope.GetMatrix(), &mModelSlope,CCollider::ETag::ESLOPE);
	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mFloor,CCollider::ETag::EFLOOR);
	mColliderMesh3.ColliderMeshSet(nullptr, nullptr, &mObject);
	

	//�r���{�[�h�̐���
	//new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);

	//UI�N���X�̐���
	spUi = new CUi();

	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();

}

void CApplication::Update()
{
	switch (mState)
	{
	//�X�^�[�g���
	case CApplication::EState::ESTART:

		//���_�����߂�
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//�����_�����߂�
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//����������߂�
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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

		mTriangle.Render();

		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::GetInstance()->Render();

		spUi->State();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			mState = EState::EPLAY;
		}

		break;

		//�Q�[����
	case CApplication::EState::EPLAY:
		//�^�X�N�}�l�[�W���̍X�V
		CTaskManager::GetInstance()->Update();
		//�R���W�����}�l�[�W���̏Փ˔���
		CTaskManager::GetInstance()->Collision();

		mPlayer.HitCollision(mPlayer.GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

		CColliderHitManager::GetInstance()->Delete();

		//���_�����߂�
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//�����_�����߂�
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//����������߂�
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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
		//mBackGround.Render();

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
		
	//�N���A���
	case CApplication::EState::ECLEAR:

		//���_�����߂�
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//�����_�����߂�
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//����������߂�
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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

		//����}�b�v�̕`��
		//mModelMap.Render();

		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::GetInstance()->Render();

		spUi->Clear();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			CUi::SetPoint();
			mState = EState::EPLAY;
		}

		break;

	//case CApplication::EState::EOVER:
	//	break;
	case CApplication::EState::ERSTART:

		

		break;
	//default:
	//	break;

	}
	

}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::GetUi()
{
	return spUi;	//�C���X�^���X�̃|�C���^��Ԃ�
}

