#include "CDoor.h"
#include "CPushSwitchManager.h"

// �R���X�g���N�^
CDoor::CDoor(const CVector& pos, const CVector& angle, const CVector& size)
	//: mpOwner(nullptr)
{
	// ���̃��f���f�[�^�擾(���͕ǂ�ݒ肵�Ă���)
	mpModel = CResourceManager::Get<CModel>("Wall");

	// ���̃R���C�_�[���擾(���͕ǂ̃R���C�_�[��ݒ肵�Ă���)
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

// �ڑ�����X�C�b�`��ݒ肷��
void CDoor::SetSwitch(size_t index)
{
	//mpOwner = CPushSwitchManager::Instance()->GetPushSwitch(index);
}

// �X�V����
void CDoor::Update()
{
	//if(mpOwner->)
}

// �`�揈��
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
