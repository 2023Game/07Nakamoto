#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"
#include "CTarget.h"

#define TIME 120

//static�̒�`
int CUi::mTime = TIME;

//�f�t�H���g�R���X�g���N�^
CUi::CUi()
	: mNum(0)
{
	mFont.Load("FontWhite.png", 1, 64);
}

CUi::~CUi()
{
	mTime = TIME;
}

void CUi::State()
{
	CCamera::Start(0, 800, 0, 600);	//2D�`��J�n
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();	//2D�`��I��
}

void CUi::Clear()
{
	CCamera::Start(0, 800, 0, 600);	//2D�`��J�n
	mFont.Draw(36, 360, 18, 36, "     GAME CREAR!!");
	//mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();	//2D�`��I��
}

//�������Ԃ̊Ǘ�
void CUi::Time()
{
	//60�t���[��������
	mNum++;

	//60�t���[���ɂȂ�����
	if (mNum == 60)
	{
		//���Ԃ�1�b���炷
		mTime--;
		mNum = 0;
	}
}

int CUi::GetTime()
{
	return mTime;
}

//�`��
void CUi::Render()
{
	CCamera::Start(0, 800, 0, 600);	//2D�`��J�n
	//�`��F�̐ݒ�(�ΐF�̔�����)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//������ҏW�G���A�̍쐬
	char buf[64];
	char str[16];

	sprintf(buf, "TARGET:%01d", CTarget::GetNum());
	sprintf(str, "TIME:%03d", mTime);

	mFont.Draw(640, 580, 8, 16, buf);
	mFont.Draw(310, 580, 10, 20, str);

	CCamera::End();	//2D�`��I��
}