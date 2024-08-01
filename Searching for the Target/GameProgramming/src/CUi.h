#ifndef CUI_H
#define CUI_H

#include "CFont.h"

class CUi
{
public:
	//�f�t�H���g�R���X�g���N�^
	CUi();

	//�f�X�g���N�^
	~CUi();

	//�X�^�[�g���
	void State();
	//�N���A���
	void Clear();

	//�������Ԃ̊Ǘ�
	void Time();
	//�������Ԃ̎擾
	static int GetTime();

	//�`��
	void Render();

private:
	//����
	static int mTime;
	//60�t���[��������
	int mNum;

	CFont mFont;
};

#endif // !CUI_H
