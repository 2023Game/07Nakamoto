#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CWarrok.h"
#include "CInteractObjectManager.h"

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
		this, ELayer::eDemon,
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
	mpCollider->Position(pos);

	mHp = HP;
	Position(pos);

	//CDemonPowerManager::Instance()->AddDemonPower(this);
	
}

// �f�X�g���N�^
CDemonPower::~CDemonPower()
{
	// �R���C�_�[���폜
	if (mpCollider != nullptr)
	{
		// �d�͂̌��̊Ǘ����X�g�����菜��
		CDemonPowerManager* dpManager = CDemonPowerManager::Instance();
		if (dpManager != nullptr)
		{
			dpManager->RemovePower(this);
		}
		SAFE_DELETE(mpCollider);
	}
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
		CWarrok::Instance()->PowerDown();
	}
}

// �`�揈��
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
