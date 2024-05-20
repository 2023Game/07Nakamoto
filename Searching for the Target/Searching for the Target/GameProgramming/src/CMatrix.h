#ifndef CMATRIX_H
#define CMATRIX_H

/*
�}�g���N�X�N���X
4�s4��̍s��f�[�^������
*/
class CMatrix {
public:
	//�\���m�F�p
	//4�~4�̍s���`��o��
	void Print();

	//�f�t�H���g�R���X�g���N�^
	CMatrix();
	//�P�ʍs��̍쐬
	CMatrix Identity();

	/// <summary>
	/// �g��k���s��̍쐬
	/// </summary>
	/// <param name="sx">�{��X</param>
	/// <param name="sy">�{��Y</param>
	/// <param name="sz">�{��Z</param>
	/// <returns></returns>
	CMatrix SetScale(float sx, float sy, float sz);

	//�s��擾
	float* GetM() const;

	//�s��l�̎擾
	//M(�s,��)
	//mM[�s][��]���擾
	float GetM(int r, int c)const;

	/// <summary>
	/// �s��l�̑��
	/// </summary>
	/// <param name="row">�s��</param>
	/// <param name="col">��</param>
	/// <param name="value">�l</param>
	void SetM(int row, int col, float value);

	/// <summary>
	/// ��]�s��(X��)
	/// </summary>
	/// <param name="degree">�p�x</param>
	/// <returns></returns>
	CMatrix SetRotateX(float degree);

	/// <summary>
	/// ��]�s��(Y��)
	/// </summary>
	/// <param name="degree">�p�x</param>
	/// <returns></returns>
	CMatrix SetRotateY(float degree);

	/// <summary>
	/// ��]�s��(Z��)
	/// </summary>
	/// <param name="degree">�p�x</param>
	/// <returns></returns>
	CMatrix SetRotateZ(float degree);

	/// <summary>
	/// �ړ��s��
	/// </summary>
	/// <param name="mx">�ړ���X</param>
	/// <param name="my">�ړ���Y</param>
	/// <param name="mz">�ړ���Z</param>
	/// <returns></returns>
	CMatrix SetTranslate(float mx, float my, float mz);

	// *���Z�q�̃I�[�o�[���[�h
	//CMatrix * CMatrix �̉��Z���ʂ�Ԃ�
	const CMatrix operator* (const CMatrix& m)const;

private:
	//4�~4�̍s��f�[�^��ݒ�
	float mM[4][4];
};

#endif // !CMATRIX_H

