#ifndef CINPUT_H
#define CINPUT_H

#include <Windows.h>

class CInput
{
public:
	CInput();

	/// <summary>
	/// bool Key(����)
	/// </summary>
	/// <param name="key">�L�[����</param>
	/// <returns>true:�L�[��������Ă���</returns>
	bool Key(char key);
private:
};

#endif // !CINPUT_H

