#include "CSlope.h"

//�f�t�H���g�R���X�g���N�^
CSlope::CSlope()
{

}

//���f���̐ݒ�
void CSlope::SetSlope(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	mColliderSlope.ColliderMeshSet(this, &mMatrix, mpModel, CCollider::ETag::ESLOPE);
}

//�X�V����
void CSlope::Update()
{
	//�ϊ��s��̍X�V
	CTransform::Update();
}