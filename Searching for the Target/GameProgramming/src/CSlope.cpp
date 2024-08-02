#include "CSlope.h"
#include "CCollisionManager.h"
#include "CCollider.h"
#include "CColliderMesh.h"

//���f���̐ݒ�
CSlope::CSlope(const CVector& pos,
	const CVector& rot, const CVector& scale, CModel* model)
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;

	//�ϊ��s��̍X�V
	CTransform::Update();
	//�R���C�_�̐���
	mCSlope.ColliderMeshSet(this, &mMatrix, CSlope::GetModelSlope(), CCollider::ETag::ESLOPE);

}

CMatrix* CSlope::GetMatrix()
{
	return &mMatrix;
}

void CSlope::Update()
{
	CTransform::Update();
}

//static�ϐ��̒�`
CModel CSlope::mModelSlope;

//��̃��f�����擾����
CModel* CSlope::GetModelSlope()
{
	return &mModelSlope;
}