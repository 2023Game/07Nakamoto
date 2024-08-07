#include "CInput.h"
#include <stdio.h>

CInput::CInput()
{
}

bool CInput::Key(char key)
{
	return GetAsyncKeyState(key) < 0;
}

GLFWwindow* CInput::mpWindow = nullptr;

//������
void CInput::Init(GLFWwindow* w)
{
	mpWindow = w;
}

//�E�B���h�E�̍�������_�Ƃ����}�E�X�J�[�\�����W���擾
void CInput::GetMousePos(int* px, int* py)
{
	double xpos, ypos;
	glfwGetCursorPos(mpWindow, &xpos, &ypos);
	*px = xpos;
	*py = ypos;
	return;
}

//�f�B�X�v���C�̍�������_�Ƃ����}�E�X�J�[�\���̍��W���擾
void CInput::GetMousePosD(int* px, int* py)
{
	POINT point;
	//WindowsAPI
	GetCursorPos(&point);
	*px = point.x;
	*py = point.y;
}

//�f�B�X�v���C�̍�������_�Ƃ����}�E�X�J�[�\���̍��W��ݒ�
void CInput::SetMousePosD(int x, int y)
{
	//WindowsAPI
	SetCursorPos(x, y);
}