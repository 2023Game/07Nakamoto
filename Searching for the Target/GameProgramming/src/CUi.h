#ifndef CUI_H
#define CUI_H

#include "CFont.h"

class CUi
{
public:
	CUi();

	//�X�^�[�g���
	void State();
	//�N���A���
	void Clear();

	//�������Ԃ̐ݒ�
	void Time();
	//�󂵂��I�̐������Z
	void AddPoint();
	//�󂵂��I�̐���������
	static void SetPoint();
	//�󂵂��I�̐����擾
	static int GetPoint();

	//�`��
	void Render();

private:
	//����
	int mTime;
	int mNum;

	static int mPoint;

	CFont mFont;
	CFont mFont2;
};

#endif // !CUI_H
