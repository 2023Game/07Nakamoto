#include "CTrap.h"
#include "CColliderMesh.h"
#include "CCharaBase.h"

CTrap::CTrap()
	: CObjectBase(ETag::eEnemy, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// ���f���f�[�^�̎擾
	mpModel = CResourceManager::Get<CModel>("Spider_Web");

	// �R���C�_�[���쐬
	mpCollider = new CColliderMesh
	(
		this, ELayer::eAttackCol,
		mpModel, true
	);
	// �v���C���[�ƏՓ˂���悤�ɐݒ�
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer });

}

// �f�X�g���N�^
CTrap::~CTrap()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

void CTrap::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �v���C���[�ɏՓ˂���
	if (other->Layer() == ELayer::ePlayer)
	{
		// �v���C���[�Ƀ_���[�W��^����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(10, this);
			// �v���C���[�ɓ���������A���g���폜
			Kill();
		}
	}
}

void CTrap::Update()
{
}

// �`��
void CTrap::Render()
{
	mpModel->Render(Matrix());
}
