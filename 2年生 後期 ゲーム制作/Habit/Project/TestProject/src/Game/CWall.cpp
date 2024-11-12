#include "CWall.h"

// �R���X�g���N�^
CWall::CWall(const CVector& pos, const CVector& angle, const CVector& size)
{
	// �ǂ̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wall");

	// �ǂ̃R���C�_�[���쐬
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, colModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

}

// �f�X�g���N�^
CWall::~CWall()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

// ���C�ƕǂƂ̏Փ˔���
bool CWall::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	//�ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpColliderMesh == nullptr) return false;

	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}

// �X�V����
void CWall::Update()
{
}

// �`�揈��
void CWall::Render()
{
	mpModel->Render(Matrix());
}
