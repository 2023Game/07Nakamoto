#ifndef CINPUT_H
#define CINPUT_H

#include "GLFW/glfw3.h"
#include <Windows.h>

class CInput
{
	//ウィンドウポインタ
	static GLFWwindow* mpWindow;

public:
	CInput();

	/// <summary>
	/// bool Key(文字)
	/// </summary>
	/// <param name="key">キー入力</param>
	/// <returns>true:キーが押されている</returns>
	bool Key(char key);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="w">ウィンドウポインタ</param>
	static void Init(GLFWwindow* w);

	/// <summary>
	/// マウスカーソルの座標を取得
	/// </summary>
	/// <param name="px">X座標ポインタ</param>
	/// <param name="py">Y座標ポインタ</param>
	static void GetMousePos(int* px, int* py);
};

#endif // !CINPUT_H

