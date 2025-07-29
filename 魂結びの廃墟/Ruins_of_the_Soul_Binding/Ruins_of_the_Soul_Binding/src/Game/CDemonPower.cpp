#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CBoss.h"
#include "CInteractObjectManager.h"
#include "CGaugeUI3D.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\break.png"
#define DEFAULT_KEY_PATH "UI\\BottonUI\\E.png"

#define HP 10	// �ϋv��

#define KYE_UI_OFFSET_Y 12.0f	// �L�[��UI�̃I�t�Z�b�g
#define GAUGE_OFFSET_Y 10.0f	// HP�Q�[�W�̃I�t�Z�b�g
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
			ETag::eCat,
			ETag::eEnemy,
		}
	);
	mpCollider->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractSearch,
			ELayer::eEnemy
		}
	);
	mpCollider->Position(OFFSET_POS);

	mMaxHp = HP;
	mHp = mMaxHp;
	Position(pos);

	mOffSetPos.Y(KYE_UI_OFFSET_Y);

	// HP�Q�[�W���쐬
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
	// �Q�[�W�̃I�t�Z�b�g�ʒu��ݒ�
	mGaugeOffsetPos.Y(GAUGE_OFFSET_Y);

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

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CDemonPower::GetInteractTextPath() const
{
	return DEFAULT_TEXT_PATH;
}

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CDemonPower::GetInteractKeyPath() const
{
	return DEFAULT_KEY_PATH;
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
