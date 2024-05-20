#ifndef CTRIANGLE_H
#define CTRIANGLE_H

#include "CVector.h"
#include "CMatrix.h"
/*
�O�p�`�N���X
*/

class CTriangle {
public:
	//�}�e���A���ԍ��̎擾
	int GetMaterialIdx();

	/// <summary>
	/// �}�e���A���ԍ��̐ݒ�
	/// </summary>
	/// <param name="idx">�}�e���A���ԍ�</param>
	void SetMaterialIdx(int idx);

	/// <summary>
	/// ���_���W�ݒ�
	/// </summary>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="v2">���_3</param>
	void SetVertex(const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// �@���ݒ�
	/// </summary>
	/// <param name="n">�@���x�N�g��</param>
	void SetNormal(const CVector& n);

	/// <summary>
	/// �@���ݒ�
	/// </summary>
	/// <param name="v0">�@���x�N�g��1</param>
	/// <param name="v1">�@���x�N�g��2</param>
	/// <param name="v2">�@���x�N�g��3</param>
	void SetNormal(const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// UV�ݒ�
	/// </summary>
	/// <param name="v0">���_1</param>
	/// <param name="v1">���_2</param>
	/// <param name="v2">���_3</param>
	void SetUV(const CVector& v0, const CVector& v1, const CVector& v2);

	//���_���WmV[0]�̎擾
	const CVector& GetV0() const;
	//���_���WmV[1]�̎擾
	const CVector& GetV1() const;
	//���_���WmV[2]�̎擾
	const CVector& GetV2() const;
	//�@��mN[0]���擾
	const CVector& GetN0() const;
	//�@��mN[1]���擾
	const CVector& GetN1() const;
	//�@��mN[2]���擾
	const CVector& GetN2() const;
	//UV mUv[0]���擾
	const CVector& GetmU0() const;
	//UV mUv[1]���擾
	const CVector& GetmU1() const;
	//UV mUv[2]���擾
	const CVector& GetmU2() const;

	//�`��
	void Render();
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="m">�s��</param>
	void Render(const CMatrix& m);

private:
	int mMaterialIdx;	//�}�e���A���ԍ�

	CVector mV[3];	//���_���W
	CVector mN[3];	//�@��

	CVector mUv[3];	//�e�N�X�`���}�b�s���O

};
#endif // !CTRIANGLE_H

