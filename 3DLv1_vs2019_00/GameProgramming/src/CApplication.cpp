#include "CApplication.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"

//�N���X��static�ϐ�
CTexture CApplication::mTexture;

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

CVector mEye;

//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
//���f���f�[�^�̎w��
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"
//�G�A���@���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"
//�G���f��(Knight)
#define MODEL_KNIGHT "res\\knight\\knight_low.x"

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//�J�����̐ݒ�
	mActionCamera.Set(5.0f, -15.0f, 180.0f);

	//3D���f���t�@�C���̓ǂݍ���
	mModelX.Load(MODEL_FILE);

	//�L�����N�^�[�Ƀ��f����ݒ�
	mXPlayer.Init(&mModelX);
	
	//mKnight.Load("res\\knight\\knight_low.x");
	mKnight.Load(MODEL_KNIGHT);
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 1:�ړ�
	mKnight.SeparateAbunatuibSet(0, 1530, 1830, "idle1");	// 2:�ҋ@
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 3:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 4:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 5:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 6:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 440, 520, "attack1");	// 7:Attack1
	mKnight.SeparateAbunatuibSet(0, 520, 615, "attack2");	// 8:Attack2
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 9:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		//10:�_�~�[
	mKnight.SeparateAbunatuibSet(0, 1160, 1260, "walk");	//11:�_�E��

	//�G�̏������ݒ�
	mXEnemy.Init(&mKnight);

	//�G�̔z�u
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));

	mXEnemy.ChangeAnimation(2, true, 200);

	mpPaladin = new CPaladin();
	mpPaladin->Position(CVector(-1.0f, 0.0f, 5.0f));
	mpPaladin->ChangeAnimation(1, true, 60);

	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{
	mpPaladin->Update();
	//�L�����N�^�[�N���X�̍X�V
	mXPlayer.Update();
	//�G�̍X�V
	mXEnemy.Update();

	//�Փˏ���
	CCollisionManager::Instance()->Collision();

	//�J�����ݒ�
	mActionCamera.Position(mXPlayer.Position()
								+ CVector(0.0f, 2.0f, 0.0f));

	mActionCamera.Update();
	mActionCamera.Render();
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//���_�ɃA�j���[�V������K�p����
	//mModelX.AnimateVertex();
	//���f���`��
	//mModelX.Render();
	mpPaladin->Render();
	mXPlayer.Render();
	//�G�`��
	mXEnemy.Render();
	//�R���C�_�̕`��
	CCollisionManager::Instance()->Render();

	//2D�`��J�n
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMMING");

	//2D�̕`��I��
	CCamera::End();
}