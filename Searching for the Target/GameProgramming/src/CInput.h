#ifndef CINPUT_H
#define CINPUT_H

#include "GLFW/glfw3.h"
#include <Windows.h>

class CInput
{
	//�E�B���h�E�|�C���^
	static GLFWwindow* mpWindow;

public:
	CInput();

	/// <summary>
	/// bool Key(����)
	/// </summary>
	/// <param name="key">�L�[����</param>
	/// <returns>true:�L�[��������Ă���</returns>
	bool Key(char key);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="w">�E�B���h�E�|�C���^</param>
	static void Init(GLFWwindow* w);

	/// <summary>
	/// �}�E�X�J�[�\���̍��W���擾
	/// </summary>
	/// <param name="px">X���W�|�C���^</param>
	/// <param name="py">Y���W�|�C���^</param>
	static void GetMousePos(int* px, int* py);
};

#endif // !CINPUT_H

