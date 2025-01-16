#include "CChoco.h"
#include "CColliderMesh.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CChoco::CChoco()
	:CObjectBase(ETag::eItem, ETaskPriority::eItem, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// ���f���f�[�^�̎擾
	mpModel = CResourceManager::Get<CModel>("Choco");

	// �R���C�_�[���쐬
	mpCollider = new CColliderMesh
	(
		this, ELayer::eItem,
		mpModel,true
	);
	
	// �v���C���[�ƏՓ˂���悤�ɐݒ�
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer });

}

// �f�X�g���N�^
CChoco::~CChoco()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

// �Փˏ���
void CChoco::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �v���C���[�ɏՓ˂���
	if (other->Layer() == ELayer::ePlayer)
	{
		// �v���C���[�ɓ���������A���g���폜
		Kill();
	}
}

// �X�V����
void CChoco::Update()
{
}

// �`�揈��
void CChoco::Render()
{
	mpModel->Render(Matrix());
}
