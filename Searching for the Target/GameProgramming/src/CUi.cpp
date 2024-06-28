#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"

CUi::CUi()
	: mPosY(0)
	//, mRotX(0)
	, mTime(0)
{
	mFont.Load("FontWhite.png", 1, 64);
}

void CUi::SetPosY(float f)
{
	mPosY = f;
}

void CUi::SetRotX(float f)
{
	//mRotX = f;
}
//���Ԃ̐ݒ�
void CUi::SetTime(int time)
{
	mTime = time;
}
//�`��
void CUi::Render()
{
	CCamera::Start(0, 800, 0, 600);	//2D�`��J�n
	//�`��F�̐ݒ�(�ΐF�̔�����)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	sprintf(buf, "PY:%7.2f", mPosY);

	mFont.Draw(500, 300, 8, 16, buf);

	CCamera::End();	//2D�`��I��
}