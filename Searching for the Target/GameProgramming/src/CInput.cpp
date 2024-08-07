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

//ウィンドウの左上を原点としたマウスカーソル座標を取得
void CInput::GetMousePos(int* px, int* py)
{
	double xpos, ypos;
	glfwGetCursorPos(mpWindow, &xpos, &ypos);
	*px = xpos;
	*py = ypos;
	return;
}

//ディスプレイの左上を原点としたマウスカーソルの座標を取得
void CInput::GetMousePosD(int* px, int* py)
{
	POINT point;
	//WindowsAPI
	GetCursorPos(&point);
	*px = point.x;
	*py = point.y;
}

//ディスプレイの左上を原点としたマウスカーソルの座標を設定
void CInput::SetMousePosD(int x, int y)
{
	//WindowsAPI
	SetCursorPos(x, y);
}