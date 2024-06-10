#ifndef CBILLBOARD_H
#define CBILLBOARD_H

#include "CCharacter.h"

/*
�r���{�[�h�N���X
��ɃJ�����̕��������l�p�`
*/

class CBillBoard :public CCharacter
{
public:
	//�f�t�H�g�R���X�g���N�^
	CBillBoard();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	CBillBoard(CVector pos, float w, float h);

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	/// <param name="priority">�D��x</param>
	CBillBoard(CVector pos, float w, float h, int priority);

	/// <summary>
	/// �r���{�[�h�̐ݒ�
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	void SetBillBoard(CVector pos, float w, float h);

	//�X�V
	void Update();

	//�`��
	void Render();
	void Render(CMaterial* mpMaterial);

protected:
	//�O�p�`2��
	CTriangle mT[2];
	//�}�e���A��
	CMaterial mMaterial;
};

#endif // !CBILLBOARD_H
