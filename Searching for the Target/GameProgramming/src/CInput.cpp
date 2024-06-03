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

//�}�E�X�J�[�\�����W���擾
void CInput::GetMousePos(int* px, int* py)
{
	double xpos, ypos;
	glfwGetCursorPos(mpWindow, &xpos, &ypos);
	*px = xpos;
	*py = ypos;
	return;
}