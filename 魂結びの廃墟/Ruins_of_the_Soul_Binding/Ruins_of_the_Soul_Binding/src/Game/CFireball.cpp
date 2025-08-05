#include "CFireball.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// �R���X�g���N�^
CFireball::CFireball(float speed, float dist)
	: CObjectBase(ETag::ePlayer, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
{
	// ���f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Fireball");

	// �R���C�_�[���쐬
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		7.0f
	);

	// �G�ƃt�B�[���h�ƏՓ˂���悤�ɐݒ�
	mpCollider->SetCollisionTags({ ETag::eEnemy, ETag::eField });
	mpCollider->SetCollisionLayers({ ELayer::eEnemy, ELayer::eFloor , ELayer::eWall });
}

// �f�X�g���N�^
CFireball::~CFireball()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpCollider);
}

// �Փˏ���
void CFireball::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �G�ɏՓ˂���
	if (other->Layer() == ELayer::eEnemy)
	{
		// �G�Ƀ_���[�W��^����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(10, this);
			// ���g���폜
			Kill();
		}
	}
	if (other->Layer() == ELayer::eWall || other->Layer() == ELayer::eFloor)
	{
		// �����ɂԂ�������A���g���폜
		Kill();
	}
}

// �X�V����
void CFireball::Update()
{
	// ���̃t���[���ňړ����������߂�
	float moveDist = mMoveSpeed * Times::DeltaTime() * 0.5f;

	// �ړ��������A�ړ�������
	CVector pos = Position();
	pos += VectorZ() * moveDist;
	Position(pos);

	// �ړ��������������Z
	mCurrDist += moveDist;
	// �ړ��o���鋗���𒴂�����A���g���폜
	if (mCurrDist >= mMoveDist)
	{
		Kill();
	}
}

// �`�揈��
void CFireball::Render()
{
	mpModel->Render(Matrix());
}

