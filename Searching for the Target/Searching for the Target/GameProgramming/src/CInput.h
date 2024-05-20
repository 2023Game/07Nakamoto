#ifndef CINPUT_H
#define CINPUT_H

#include <Windows.h>

class CInput
{
public:
	CInput();

	/// <summary>
	/// bool Key(文字)
	/// </summary>
	/// <param name="key">キー入力</param>
	/// <returns>true:キーが押されている</returns>
	bool Key(char key);
private:
};

#endif // !CINPUT_H

