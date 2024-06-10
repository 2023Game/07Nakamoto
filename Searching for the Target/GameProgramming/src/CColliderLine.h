#ifndef CCOLLIDERLINE_H
#define CCOLLIDERLINE_H

#include "CCollider.h"

/*
�����R���C�_�̒�`
*/

class CColliderLine :public CCollider
{
public:

	enum class ESType
	{
		ELine1,
		ELine2,
		ELine3,
	};

	//�f�t�H���g�R���X�g���N�^
	CColliderLine() {}
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	CColliderLine(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);

	/// <summary>
	/// �����R���C�_�̐ݒ�
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	void ColliderLineSet(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);

	//�`��
	void Render();

	//�D��x�̕ύX
	void ChangePriority();
};

#endif // !CCOLLIDERLINE_H

