#ifndef CCOLLIDERTRIANGLE_H
#define CCOLLIDERTRIANGLE_H

#include "CCollider.h"

/*
�O�p�`�R���C�_�̒�`
*/

class CColliderTriangle :public CCollider
{
public:
	//�f�t�H���g�R���X�g���N�^
	CColliderTriangle() {}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="v2">���_3</param>
	CColliderTriangle(CCharacter* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// �O�p�R���C�_�̐ݒ�
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="v2">���_3</param>
	void SetColliderTriangle(CCharacter* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);

	//�`��
	void Render();

	//�D��x�̕ύX
	void ChangePriority();

};

#endif // !CCOLLIDERTRIANGLE_H

