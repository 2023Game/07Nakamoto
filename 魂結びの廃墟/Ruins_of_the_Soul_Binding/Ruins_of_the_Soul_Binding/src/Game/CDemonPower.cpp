#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CBoss.h"
#include "CInteractObjectManager.h"
#include "CGaugeUI3D.h"

#define HP 10	// �ϋv��
#define GAUGE_OFFSET_Y 10.0f
#define OFFSET_POS CVector(0.0f, 1.5f, 0.0f)	// �R���C�_�[�̃I�t�Z�b�g

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
	mpCollider->Position(OFFSET_POS);

	mMaxHp = HP;
	mHp = mMaxHp;
	Position(pos);

	// HP�Q�[�W���쐬
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
	// �Q�[�W�̃I�t�Z�b�g�ʒu��ݒ�
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	CDemonPowerManager::Instance()->AddDemonPower(this);
	
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

	// HP�Q�[�W�����݂�����A�ꏏ�ɍ폜����
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// ���ׂ�
void CDemonPower::Interact()
{

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
		CBoss::Instance()->PowerDown();
	}

	// HP�Q�[�W�����݂���΁A
	if (mpHpGauge != nullptr)
	{
		// HP�Q�[�W���X�V
		mpHpGauge->Position(Position() + mGaugeOffsetPos);
		mpHpGauge->SetMaxPoint(mMaxHp);
		mpHpGauge->SetCurrPoint(mHp);
	}

}

// �`�揈��
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
