#include "CObject.h"

//�R���X�g���N�^
CObject::CObject(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//�ϊ��s��̍X�V
	CTransform::Update();
	//�R���C�_�̐���
	mCObject.ColliderMeshSet(this, &mMatrix, CObject::GetModelObject(), CCollider::ETag::ESLOPE);
}

//static�ϐ��̒�`
CModel CObject::mModelObject;
//�I�u�W�F�N�g�̃��f�����擾����
CModel* CObject::GetModelObject()
{
	return &mModelObject;
}