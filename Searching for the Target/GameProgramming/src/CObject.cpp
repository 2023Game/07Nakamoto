#include "CObject.h"

//コンストラクタ
CObject::CObject(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//変換行列の更新
	CTransform::Update();
	//コライダの生成
	mCObject.ColliderMeshSet(this, &mMatrix, CObject::GetModelObject(), CCollider::ETag::ESLOPE);
}

//static変数の定義
CModel CObject::mModelObject;
//オブジェクトのモデルを取得する
CModel* CObject::GetModelObject()
{
	return &mModelObject;
}