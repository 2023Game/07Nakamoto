#include "CInput.h"
#include <stdio.h>

CInput::CInput()
{
}

bool CInput::Key(char key)
{
	return GetAsyncKeyState(key) < 0;
}