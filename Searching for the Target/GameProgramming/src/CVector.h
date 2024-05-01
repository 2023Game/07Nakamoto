#ifndef CVECTOR_H
#define CVECTOR_H
/*
�x�N�g���N���X
�x�N�g���f�[�^������
*/
class CVector {
public:
	//�f�t�H���g�R���X�g���N�^
	CVector();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="z">Z���W</param>
	CVector(float x, float y, float z);

	/// <summary>
	/// �e���ł̒l�̐ݒ�
	/// </summary>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	/// <param name="z">z���W</param>
	void Set(float x, float y, float z);

	//X�̒l�𓾂�
	float GetX() const;
	//Y�̒l�𓾂�
	float GetY() const;
	//Z�̒l�𓾂�
	float GetZ() const;

	//+���Z�q�̃I�[�o�[���[�h
	//CVector + CVector�̉��Z���ʂ�Ԃ�
	CVector operator+(const CVector& v)const;

	//-���Z�q�̃I�[�o�[���[�h
	//CVector - CVector�̉��Z���ʂ�Ԃ�
	CVector operator-(const CVector& v)const;

private:
	//3D�e���ł̒l��ݒ�
	float mX, mY, mZ;
};

#endif // !CVECTOR_H

