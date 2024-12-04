#include "CDoor.h"

// �R���X�g���N�^
CDoor::CDoor(CObjectBase* owner, const CVector& pos, const CVector& angle, const CVector& size)
{
	// �X�C�b�`�̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Wall");

	// �X�C�b�`�̃R���C�_�[���擾
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CDoor::~CDoor()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

// �X�V����
void CDoor::Update()
{
}

// �`�揈��
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
