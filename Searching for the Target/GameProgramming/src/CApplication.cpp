#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMovingDoor.h"

#include "CCamera.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//�v���C���[
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//�I
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//�e

#define MODEL_MAP "res\\map3.obj","res\\map3.mtl"			//����}�b�v
//#define MODEL_MAP "res\\SlopeMap2.obj","res\\SlopeMap2.mtl"			//����}�b�v
#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"				//�w�i��

#define MODEL_SLOPE "res\\Slope.obj","res\\Slope.mtl"		//��

#define MODEL_SPHERE "res\\sphere.obj" ,"res\\sphere.mtl"	//��(�X�C�b�`)

#define MODEL_BLUECUBE "res\\BlueCube.obj" ,"res\\BlueCube.mtl"	//�F�̎l�p�`

//#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"	//�w�i���f��

CApplication::~CApplication()
{
	delete spUi;	//�C���X�^���X�̍폜
}

void CApplication::Start()
{
	//�J�[�\���̍��W��ݒ肷��
	//SetCursorPos(400, 300);

	//���f���t�@�C���̓���
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	mModelMap.Load(MODEL_MAP);
	mBackGround.Load(MODEL_SKY);
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
	new CTarget(&mModelTarget, CVector(-20.0f, 5.0f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -50.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);
	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -25.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);
	/*
	//�X�C�b�`�Ɣ��̐���
	mSwitch.SetSwitch(&mSwitch, &mModelSwitch,
		CVector(0.5f, 3.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(1.0f, 1.0f, -6.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	mSwitch2.SetSwitch(&mSwitch2, &mModelSwitch,
		CVector(0.5f, 3.0f, -4.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(1.0f, 3.0f, -6.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	*/

	//�X�C�b�`�̐���
	mSwhith3.SetSwitch(&mModelSwitch,
		CVector(0.5f, 3.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//���̐���
	mDoor.SetMovingDoor(&mSwhith3, CVector(1.0f, 3.0f, -6.0f), CVector(0.0f, 0.0f, 0.0f), 
		CVector(1.0f, 1.0f, 1.0f));


	//��̐���
	mSlope.SetSlope(CVector(-5.0f, 5.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f), &mModelSlope);

	//���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	//�w�i�̃��f��
	mColliderMesh.ColliderMeshSet(nullptr, nullptr, &mBackGround);
	//�X�e�[�W�̃��f��
	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mModelMap);
	
	//�r���{�[�h�̐���
	new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);

	//UI�N���X�̐���
	spUi = new CUi();
}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();
	//�R���W�����}�l�[�W���̏Փ˔���
	CTaskManager::GetInstance()->Collision();

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;	//���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();
	
	// �m�F�p�̎��_ �E
	//e = mPlayer.GetPosition() + CVector(-10.0f, 1.0f, 0.0f) * mPlayer.GetMatrixRotate();

	//e = mPlayer.GetPosition() + CVector(-5.0f, 1.0f, 3.0f) * mPlayer.GetMatrixRotate();

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

	CApplication::Ui()->SetPosY(mPlayer.GetPosition().GetY());

	//�w�i
	mBackGround.Render();

	//����}�b�v�̕`��
	mModelMap.Render(); 

	//�^�X�N���X�g�̍폜
	CTaskManager::GetInstance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::GetInstance()->Render();
	//�R���W�����}�l�[�W���̕`��
	CCollisionManager::GetInstance()->Render();

	//2D�`��J�n
	//CCamera::Start(0, 800, 0, 600);

	spUi->Render();

	//2D�`��I��
	//CCamera::End();

}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;	//�C���X�^���X�̃|�C���^��Ԃ�
}