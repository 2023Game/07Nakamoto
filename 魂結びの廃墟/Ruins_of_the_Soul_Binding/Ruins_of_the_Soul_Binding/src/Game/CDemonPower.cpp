#include "CDemonPower.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CWarrok.h"

#define HP 10	// �ϋv��

// �R���X�g���N�^
CDemonPower::CDemonPower(const CVector& pos)
	:mBroken(false)
{
	// ���f���f�[�^�̎擾
	mpModel = CResourceManager::Get<CModel>("DemonPower");

	// �R���C�_�[��ݒ�
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		4.0f, true
	);

	mpCollider->SetCollisionTags
	( 
		{
			ETag::ePlayer,
			ETag::eEnemy,
		}
	);
	mpCollider->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy
		}
	);

	mHp = HP;
	Position(pos);

}

// �f�X�g���N�^
CDemonPower::~CDemonPower()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

// ���ׂ�
void CDemonPower::Interact()
{
	mHp -= 5;
}

// �j�󂳂ꂽ��
bool CDemonPower::BreakPower()
{
	return mBroken;
}

// �X�V����
void CDemonPower::Update()
{
	if (mHp <= 0)
	{
		Kill();
	}
}

// �`�揈��
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
