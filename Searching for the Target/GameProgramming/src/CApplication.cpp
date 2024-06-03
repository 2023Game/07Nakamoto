#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//�v���C���[
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//�I
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//�e

#define MODEL_MAP "res\\map.obj","res\\map.mtl"				//����}�b�v
//#define MODEL_F14 "res\\f14.obj","res\\f14.mtl"
#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"

//#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"			//�w�i���f��

void CApplication::Start()
{
	//���f���t�@�C���̓���
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	// �폜 mModelBullet.Load(MODEL_BULLET);
	mModelMap.Load(MODEL_MAP);
	mBackGround.Load(MODEL_SKY);

	CBullet::GetModelBullet()->Load(MODEL_BULLET);

	mEye = CVector(0.0f, 2.0f, 13.0f);

	CMatrix matrix;
	matrix.Print();

	mPlayer.SetModel(&mModel);
	mPlayer.SetScale(CVector(1.5f, 1.5f, 1.5f));
	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));
	
	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -75.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -50.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -25.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͖���
	mColliderMesh.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mModelMap);

}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();
	//�R���W�����}�l�[�W���̏Փ˔���
	// �폜 CCollisionManager::GetInstance()->Collison();
	CTaskManager::GetInstance()->Collision();

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;	//���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.GetPosition() + CVector(0, 5, -10) * mPlayer.GetMatrixRotate();
	
	// �m�F�p�̎��_ �E
	//e = mPlayer.GetPosition() + CVector(-20.0f, 5.0f, 0.0f) * mPlayer.GetMatrixRotate();

	//�����_�����߂�
	c = mPlayer.GetPosition() + CVector(0.0f,0.0f,0.0f);
	//����������߂�
	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
	//�J�����ݒ�
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());

	mBackGround.Render();

	//����}�b�v�̕`��
	mModelMap.Render(); 

	//�^�X�N���X�g�̍폜
	CTaskManager::GetInstance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::GetInstance()->Render();
	//�R���W�����}�l�[�W���̕`��
	CCollisionManager::GetInstance()->Render();

}