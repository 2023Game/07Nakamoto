#ifndef CUI_H
#define CUI_H

#include "CFont.h"

class CUi
{
public:
	CUi();

	void SetPosY(float f);
	void SetRotX(float f);

	//���Ԃ̐ݒ�
	void SetTime(int time);

	//�`��
	void Render();

private:
	float mPosY;
	//float mRotX;

	int mTime;

	CFont mFont;
};

#endif // !CUI_H
