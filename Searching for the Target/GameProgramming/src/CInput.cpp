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

//初期化
void CInput::Init(GLFWwindow* w)
{
	mpWindow = w;
}

//マウスカーソル座標を取得
void CInput::GetMousePos(int* px, int* py)
{
	double xpos, ypos;
	glfwGetCursorPos(mpWindow, &xpos, &ypos);
	*px = xpos;
	*py = ypos;
	return;
}