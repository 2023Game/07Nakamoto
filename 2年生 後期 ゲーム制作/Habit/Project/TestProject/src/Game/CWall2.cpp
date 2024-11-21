#include "CWall2.h"

// �R���X�g���N�^
CWall2::CWall2(const CVector& pos, const CVector& angle, const CVector& size)
{
	// �ǂ̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Map_mini");

	// �ǂ̃R���C�_�[���쐬
	//CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	//mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

}

// �f�X�g���N�^
CWall2::~CWall2()
{
	// �R���C�_�[���폜
	//SAFE_DELETE(mpColliderMesh);
}

// ���C�ƕǂƂ̏Փ˔���
bool CWall2::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	//�ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpColliderMesh == nullptr) return false;

	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}

// �X�V����
void CWall2::Update()
{
}

// �`�揈��
void CWall2::Render()
{
	mpModel->Render(Matrix());
}
