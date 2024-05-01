#include "CApplication.h"
#include "CVector.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\untitled.obj", "res\\untitled.mtl"				//�v���C���[
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"		//�I

#define MODEL_F14 "res\\f14.obj","res\\f14.mtl"
#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"

//#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"			//�w�i���f��

void CApplication::Start()
{
	//���f���t�@�C���̓���
	//mModel.Load(MODEL_TANK);
	//mModel.Load(MODEL_TARGET);
	 
	//mBackGround.Load(MODEL_BAKCGROUND);

	mModel.Load(MODEL_F14);
	mBackGround.Load(MODEL_SKY);

	mEye = CVector(0.0f, 2.0f, 13.0f);
}

void CApplication::Update()
{
	//���_�̐ݒ�
	//gluLookAt(���_X, ���_Y, ���_Z, ���SX, ���SY, ���SZ, ���X, ���Y, ���Z)
	gluLookAt(mEye.GetX(), mEye.GetY(), mEye.GetZ(), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

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
		mEye = mEye - CVector(0.1f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.1f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.1f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.1f);
	}

	mModel.Render();

	mBackGround.Render();
}
