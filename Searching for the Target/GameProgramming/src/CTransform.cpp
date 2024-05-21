#include "CTransform.h"

//�ʒu�̎擾
const CVector& CTransform::GetPosition() const
{
	return mPosition;
}
//�ʒu�̐ݒ�
void CTransform::SetPosition(const CVector& v)
{
	mPosition = v;
}
//��]�l�̎擾
const CVector& CTransform::GetRotation() const
{
	return mRotation;
}
//��]�l�̐ݒ�
void CTransform::SetRotation(const CVector& v)
{
	mRotation = v;
}
//�g��k���l�̐ݒ�
void CTransform::SetScale(const CVector& v)
{
	mScale = v;
}
//�����s��̎擾
const CMatrix& CTransform::GetMatrix() const
{
	return mMatrix;
}
//��]�s��̎擾
const CMatrix& CTransform::GetMatrixRotate() const
{
	return mMatrixRotate;
}
//�ʒu�A��]�A�g�k�̍X�V
void CTransform::Update(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	Update();
}
//�s��X�V����
void CTransform::Update()
{
	//�g��k���s��̐ݒ�
	mMatrixScale.SetScale(mScale.GetX(), mScale.GetY(), mScale.GetZ());
	//��]�s��̐ݒ�
	mMatrixRotate =
		CMatrix().SetRotateZ(mRotation.GetZ()) *
		CMatrix().SetRotateX(mRotation.GetX()) *
		CMatrix().SetRotateY(mRotation.GetY());
	//���s�ړ��s��̐ݒ�
	mMatrixTranslate.SetTranslate(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());
	//�����s��̐ݒ�
	mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;
}
