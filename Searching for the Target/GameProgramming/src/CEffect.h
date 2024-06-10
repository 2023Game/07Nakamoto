#ifndef CEFFECT_H
#define CEFFECT_H

#include "CBillBoard.h"

/*
�G�t�F�N�g�N���X
�e�N�X�`���̃A�j���[�V����
*/

class CEffect :public CBillBoard
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	/// <param name="texture">�e�N�X�`����</param>
	/// <param name="row">�s��</param>
	/// <param name="col">��</param>
	/// <param name="fps">1�R�}������̃t���[����</param>
	CEffect(const CVector& pos, float w, float h, char* texture, int row = 1, int col = 1, int fps = 1);
	//�s���A�񐔁A�t���[�����̓f�t�H���g�����ŌĂяo�����ȗ��\

	//�X�V
	void Update();
	//�`��
	void Render();

private:
	//�s��
	int mRows;
	//��
	int mCols;
	//1�R�}�̃t���[����
	int mFps;
	//�t���[���J�E���^
	int mFrame;
	//�}�e���A��
	static CMaterial sMaterial;
};

#endif // !CEFFECT_H

