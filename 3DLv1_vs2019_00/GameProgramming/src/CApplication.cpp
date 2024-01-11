#include "CApplication.h"
#include "CRectangle.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
//OpenGL
#include "glut.h"

//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

CVector mEye;

//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
//���f���f�[�^�̎w��
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"
//�G�A���@���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"

CApplication::~CApplication()
{
	delete spUi;	//�C���X�^���XUi�̍폜
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi;
}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	new CEnemy3(CVector(-5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy3(CVector(5.0f, 1.0f, -10.0f), CVector(), CVector(0.1f, 0.1f, 0.1f));

	spUi = new CUi();	//UI�N���X�̐���

	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.1f, 0.1f, 0.1f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));

	CMatrix matrix;
	matrix.Print();

	mEye = CVector(1.0f, 2.0f, 3.0f);

	mBackGround.Load(MODEL_BACKGROUND);
	//���f���t�@�C���̓���
	mModel.Load(MODEL_OBJ);

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);

	//C5���f���̓ǂݍ���
	mModelC5.Load(MODEL_C5);
	//�G�@�̃C���X�^���X�쐬
	new CEnemy(&mModelC5, CVector(0.0f, 10.0f, -100.0f), 
		CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CEnemy(&mModelC5, CVector(30.0f, 10.0f, -130.0f),
		CVector(), CVector(0.1f, 0.1f, 0.1f));
	//�r���{�[�h�̐���
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
	//�폜 CCollisionManager::Instance()->Collision();
	CTaskManager::Instance()->Collision();

	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}

	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}

	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}

	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}

	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}

	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
	}

	//���_1�A���_2�A���_3�A�@���f�[�^�̍쐬
	CVector v0, v1, v2, n;
	//�@����������Őݒ肷��
	n.Set(0.0f, 1.0f, 0.0f);
	//���_1�̍��W��ݒ肷��
	v0.Set(0.0f, 0.0f, 0.5f);
	//���_2�̍��W��ݒ肷��
	v1.Set(1.0f, 0.0f, 0.0f);
	//���_3�̍��W��ݒ肷��
	v2.Set(0.0f, 0.0f, -0.5f);

	//mPlayer.Update();

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.Position() + CVector(-0.2f, 1.0f, -3.0f) * mPlayer.MatrixRotate();
	//�����_�����߂�
	c = mPlayer.Position();
	//����������߂�
	u = CVector(0, 1, 0) * mPlayer.MatrixRotate();
	//�J�����ݒ�
	//gluLookAt(���_X,���_Y,���_Z,���SX,���SY,���SZ,���X,���Y,���Z)
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);
	
	//mPlayer.Render();

	mBackGround.Render();

	//�^�X�N�}�l�[�W���̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	//�R���C�_�̕`��
	CCollisionManager::Instance()->Render();

	spUi->Render();		//UI�̕`��
}