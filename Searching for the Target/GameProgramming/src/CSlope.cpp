#include "CSlope.h"
#include "CCollisionManager.h"
#include "CCollider.h"

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

	//mColliderSlope.ColliderMeshSet(this, &mMatrix, mpModel);
}

//�X�V����
void CSlope::Update()
{
	//�ϊ��s��̍X�V
	CTransform::Update();
}

void CSlope::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mColliderSlope.ChangePriority();
	//�Փˏ����̎��s
	CCollisionManager::GetInstance()->Collision(&mColliderSlope, COLLISIONRANGE);
}

CMatrix* CSlope::GetMatrix()
{
	return &mMatrix;
}

