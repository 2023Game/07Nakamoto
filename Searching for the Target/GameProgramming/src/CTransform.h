#ifndef CTRANSFORM_H
#define CTRANSFORM_H

#include "CVector.h"
/*
�g�����X�t�H�[���N���X
�ʒu�A��]�A�g�k�A�ϊ��s��̊�{�N���X
*/
class CTransform {
public:
	//�ʒu�̎擾
	const CVector& GetPosition() const;
	
	/// <summary>
	/// �ʒu�̐ݒ�
	/// </summary>
	/// <param name="v">�ʒu</param>
	void SetPosition(const CVector& v);

	//��]�s��̎擾
	const CMatrix& GetMatrixRotate() const;

	//��]�l�̎擾
	const CVector& GetRotation() const;

	/// <summary>
	/// ��]�l�̐ݒ�
	/// </summary>
	/// <param name="v">��]�l</param>
	void SetRotation(const CVector& v);

	/// <summary>
	/// �g��k���̐ݒ�
	/// </summary>
	/// <param name="v">�g��k���l</param>
	void SetScale(const CVector& v);

	//�����s��̎擾
	const CMatrix& GetMatrix() const;

	//�s��X�V����
	void Update();

	/// <summary>
	/// �ʒu�A��]�A�g�k�̍X�V
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	void Update(const CVector& pos, const CVector& rot, const CVector& scale);

protected:	//�q�N���X�̓A�N�Z�X�\
	CVector mPosition;			//�ʒu
	CVector mRotation;			//��]
	CVector mScale;				//�g��k��
	CMatrix mMatrixTranslate;	//���s�ړ��s��
	CMatrix mMatrixRotate;		//��]�s��
	CMatrix mMatrixScale;		//�g��k���s��
	CMatrix mMatrix;			//�����s��
	
};

#endif // !CTRANSFORM_H

