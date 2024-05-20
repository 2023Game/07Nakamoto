#include "CTransform.h"

//位置の取得
const CVector& CTransform::GetPosition() const
{
	return mPosition;
}
//位置の設定
void CTransform::SetPosition(const CVector& v)
{
	mPosition = v;
}
//回転値の取得
const CVector& CTransform::GetRotation() const
{
	return mRotation;
}
//回転値の設定
void CTransform::SetRotation(const CVector& v)
{
	mRotation = v;
}
//拡大縮小値の設定
void CTransform::SetScale(const CVector& v)
{
	mScale = v;
}
//合成行列の取得
const CMatrix& CTransform::GetMatrix() const
{
	return mMatrix;
}
//回転行列の取得
const CMatrix& CTransform::GetMatrixRotate() const
{
	return mMatrixRotate;
}
//位置、回転、拡縮の更新
void CTransform::Update(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	Update();
}
//行列更新処理
void CTransform::Update()
{
	//拡大縮小行列の設定
	mMatrixScale.SetScale(mScale.GetX(), mScale.GetY(), mScale.GetZ());
	//回転行列の設定
	mMatrixRotate =
		CMatrix().SetRotateZ(mRotation.GetZ()) *
		CMatrix().SetRotateX(mRotation.GetX()) *
		CMatrix().SetRotateY(mRotation.GetY());
	//平行移動行列の設定
	mMatrixTranslate.SetTranslate(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ());
	//合成行列の設定
	mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;
}
