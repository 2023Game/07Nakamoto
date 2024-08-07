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
	/// �E�B���h�E�̍��オ���_(0,0)
	/// </summary>
	/// <param name="px">X���W�|�C���^</param>
	/// <param name="py">Y���W�|�C���^</param>
	static void GetMousePos(int* px, int* py);

	/// <summary>
	/// �}�E�X�J�[�\���̍��W���擾
	/// �f�B�X�v���C�̍��オ���_(0,0)
	/// </summary>
	/// <param name="px">X���W�|�C���^</param>
	/// <param name="py">Y���W�|�C���^</param>
	static void GetMousePosD(int* px, int* py);

	/// <summary>
	/// �}�E�X�J�[�\���̍��W��ݒ肷��
	/// �f�B�X�v���C�̍��オ���_(0,0) 
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	static void SetMousePosD(int x, int y);
};

#endif // !CINPUT_H

