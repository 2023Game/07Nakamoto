#include "CGhost.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",		true,	59.0f,	1.0f	},	// �ҋ@
};

// �R���X�g���N�^
CGhost::CGhost()
	: mpBattleTarget(nullptr)
{
	// �G��������
	InitEnemy("Ghost", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// �{�̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// �t�B�[���h�ƁA�v���C���[�̍U���R���C�_�[�ƃq�b�g����悤�ɐݒ�
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eAttackCol });

}

// �f�X�g���N�^
CGhost::~CGhost()
{
}

// �U������
bool CGhost::IsAttacking() const
{
	return false;
}

// �U���J�n
void CGhost::AttackStart()
{
}

// �U���I��
void CGhost::AttackEnd()
{
}

// �Փˏ���
void CGhost::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �x�[�X�̏Փˏ������Ăяo��
	CEnemy::Collision(self, other, hit);

}

// �X�V
void CGhost::Update()
{
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch ((EState)mState)
	{
		// �ҋ@���
	case EState::eIdle:		UpdateIdle();	break;
		// �ǂ�������
	case EState::eChase:	UpdateChase();	break;
	}

}

// �퓬����̕��֌���
void CGhost::LookAtBattleTarget(bool immediate)
{
}

// ��Ԑ؂�ւ�
void CGhost::ChangeState(int state)
{
	// �U�����ɑ��̏�Ԃֈڍs����ꍇ��
	// �U���I���������Ăяo��
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// ��Ԑ؂�ւ�
	CEnemy::ChangeState(state);

}

void CGhost::UpdateIdle()
{
	// �ʏ펞�̑ҋ@
	ChangeAnimation((int)EAnimType::eIdle);

	// �G�̃x�[�X�N���X�̍X�V
	CEnemy::Update();
}

// �ǂ������鎞�̍X�V����
void CGhost::UpdateChase()
{
	//mMoveSpeed = CVector::zero;

	//// ���ݒn�ƖړI�n���擾
	//CVector pos = Position();
	//CVector targetPos = mpBattleTarget->Position();
	//targetPos.Y(pos.Y());
	//// ���ݒn����ړI�n�܂ł̃x�N�g�������߂�
	//CVector vec = targetPos - pos;
	//// �U���͈͓��ł����
	//float dist = vec.Length();
	//if (dist <= ATTACK_RANGE)
	//{
	//	// �U����Ԃֈڍs
	//	ChangeState((int)EState::eAttack1);
	//}
	//// �U���͈͊O
	//else
	//{
	//	// ���s�A�j���[�V�������Đ�
	//	ChangeAnimation((int)EAnimType::eWalk);

	//	// �c�苗�����ړ��������傫���ꍇ�́A�ړ��������ړ�
	//	CVector dir = vec.Normalized();
	//	float moveDist = CHAISE_SPEED * Times::DeltaTime();
	//	if (dist > moveDist)
	//	{
	//		mMoveSpeed = dir * moveDist;
	//	}
	//	// �c�苗���̕����������ꍇ�́A
	//	// �c�苗�����ړ����āA�ҋ@��Ԃֈڍs
	//	else
	//	{
	//		mMoveSpeed = dir * dist;
	//		ChangeState((int)EState::eIdle);
	//	}
	//}

	//// ���X�ɐ퓬����̕����֌���
	//LookAtBattleTarget();

}
