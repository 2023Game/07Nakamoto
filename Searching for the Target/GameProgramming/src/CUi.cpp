#include <stdio.h>
#include <glut.h>
#include "CUi.h"
#include "CCamera.h"

int CUi::mPoint = 0;

//�f�t�H���g�R���X�g���N�^
CUi::CUi()
	: mNum(0)
	, mTime(120)
{
	mFont.Load("FontWhite.png", 1, 64);
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

//���Ԃ̐ݒ�
void CUi::Time()
{
	mNum++;

	if (mNum == 60)
	{
		mTime--;
		mNum = 0;
	}
}
//�|�C���g�����Z
void CUi::AddPoint()
{
	mPoint++;
}
//�|�C���g�̎擾
int CUi::GetPoint()
{
	return mPoint;
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

	sprintf(buf, "POINT:%01d", mPoint);
	sprintf(str, "TIME:%03d", mTime);

	mFont.Draw(640, 580, 8, 16, buf);
	mFont.Draw(310, 580, 10, 20, str);

	CCamera::End();	//2D�`��I��
}