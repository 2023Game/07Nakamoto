#include "CGhost.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
//#include "CDebugFieldOfView.h"
#include "CPlayer2.h"
#include "CField.h"
#include "Primitive.h"
#include "CTrap.h"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

#define WALK_SPEED		50.0f	// �������x
#define ROTATE_SPEED	6.0f	// ��]���x

#define FOV_ANGLE			 45.0f	// ����͈͂̊p�x
#define FOV_LENGTH			100.0f	// ����͈͂̋���
#define EYE_HEIGHT			 10.0f	// ���_�̍���

#define PATROL_INTERVAL		  3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST	 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME			  5.0f	// �ҋ@��Ԃ̎���

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",		true,	59.0f,	1.0f	},	// �ҋ@
};

// �R���X�g���N�^
CGhost::CGhost(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	//, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
	, mTrap(false)
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
	// ������R���C�_�[�̐ݒ�
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer , ETag::eTrap});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eTrap });

//#if _DEBUG
//	// ����͈͂̃f�o�b�O�\���N���X���쐬
//	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
//#endif

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// ����|�C���g�Ɍo�H�T���p�̃m�[�h��z�u
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

}

// �f�X�g���N�^
CGhost::~CGhost()
{
	//// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜����
	//if (mpDebugFov != nullptr)
	//{
	//	mpDebugFov->SetOwner(nullptr);
	//	mpDebugFov->Kill();
	//}

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);

		// ����m�[�h�ɔz�u�����m�[�h���S�č폜
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPoints.erase(itr);
			delete del;
		}
	}
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

	if (self == mpBodyCol)
	{
		if (other->Layer() == ELayer::eTrap)
		{
			mIsTrap = true;
		}
		else if (other->Layer() != ELayer::eTrap)
		{
			mIsTrap = false;
		}
	}
}

// �I�u�W�F�N�g�폜��`����֐�
void CGhost::DeleteObject(CObjectBase* obj)
{
	//// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���ł����
	//// ���g���폜���ꂽ
	//if (obj == mpDebugFov)
	//{
	//	mpDebugFov = nullptr;
	//}
}

// �X�V
void CGhost::Update()
{
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch ((EState)mState)
	{
		// �ҋ@���
	case EState::eIdle:		UpdateIdle();	break;
		// �p�j
	case EState::ePatrol:	UpdateePatrol(); break;
		// �ǂ�������
	case EState::eChase:	UpdateChase();	break;
	}

	// �G�̃x�[�X�N���X�̍X�V
	CEnemy::Update();

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

// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
bool CGhost::IsFoundPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A����͈͊O�Ƃ���
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;

	// �v���C���[�����S��Ԃ̏ꍇ���A�͈͊O�Ƃ���
	if (player->GetState() == 8) return false;

	// �v���C���[���W���擾
	CVector playerPos = player->Position();
	// �������g�̍��W���擾
	CVector pos = Position();
	// ���g����v���C���[�܂ł̃x�N�g�������߂�
	CVector  vec = playerPos - pos;
	vec.Y(0.0f);	//�v���C���[�Ƃ̍����̍����l�����Ȃ�

	// �@ ����p�x�������߂�
	// �x�N�g���𐳋K�����Ē�����1�ɂ���
	CVector dir = vec.Normalized();
	// ���g�̐��ʕ����x�N�g�����擾
	CVector forward = VectorZ();
	// �v���C���[�Ƃ܂ł̃x�N�g����
	// ���g�̐��ʕ����x�N�g���̓��ς����߂Ċp�x���o��
	float dot = CVector::Dot(dir, forward);
	// ����p�x�̃��W�A�������߂�
	float angleR = Math::DegreeToRadian(mFovAngle);
	// ���߂����ςƎ���p�x�ŁA����͈͓������f����
	if (dot < cosf(angleR))	return false;

	// �A ���싗���������߂�
	//�v���C���[�܂ł̋����Ǝ��싗���ŁA����͈͓������f����
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// �v���C���[�Ƃ̊ԂɎՕ������Ȃ����`�F�b�N����
	if (!IsLookPlayer()) return false;

	//�S�Ă̏������N���A�����̂ŁA����͈͓��ł���
	return true;
}

// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
bool CGhost::IsLookPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A�����Ȃ�
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;
	// �t�B�[���h�����݂��Ȃ��ꍇ�́A�Օ������Ȃ��̂Ō�����
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// �v���C���[�̍��W���擾
	CVector playerPos = player->Position() + offsetPos;
	// �������g�̍��W���擾
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	//�t�B�[���h�ƃ��C������s���A�Օ��������݂����ꍇ�́A�v���C���[�������Ȃ�
	if (field->CollisionRay(selfPos, playerPos, &hit)) return false;

	// �v���C���[�Ƃ̊ԂɎՕ������Ȃ��̂ŁA�v���C���[�������Ă���
	return true;
}


bool CGhost::MoveTo(const CVector& targetPos, float speed)
{
	// �ړI�n�܂ł̃x�N�g�������߂�
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// �ړ������x�N�g�������߂�
	CVector moveDir = vec.Normalized();

	// ���X�Ɉړ������ֈړ�
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// ���݂̐��ʕ���
		moveDir,	// �ړ�����
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// ����̈ړ����������߂�
	float moveDist = speed * Times::DeltaTime();
	// �ړI�n�܂ł̎c��̋��������߂�
	float remainDist = vec.Length();
	// �c��̋������ړ��������Z���ꍇ
	if (remainDist <= moveDist)
	{
		// �ړI�n�܂ňړ�����
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// �ړI�n�ɓ��������̂ŁAtrue��Ԃ�
	}

	// �c��̋������ړ�������蒷���ꍇ�́A
	// �ړ��������ړI�n�ֈړ�����
	pos += moveDir * moveDist;
	Position(pos);

	// �ړI�n�ɂ͓������Ȃ�����
	return false;
}

// ���ɏ��񂷂�|�C���g��ύX����
void CGhost::ChangePatrolPoint()
{
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�͏������Ȃ�
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// ����J�n���ł���΁A��ԋ߂�����|�C���g��I��
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;		// ��ԋ߂�����|�C���g�̔ԍ�
		float nearDist = 0.0f;	// ��ԋ߂�����|�C���g�܂ł̋���
		// �S�Ă̏���|�C���g�̋����𒲂ׁA��ԋ߂�����|�C���g��T��
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// ����|�C���g���߂�����ꍇ�́A�X���[
			if (dist < PATROL_NEAR_DIST) continue;
			// ��ԍŏ��̏���|�C���g�������́A
			// ���݈�ԋ߂�����|�C���g��肳��ɋ߂��ꍇ�́A
			// ����|�C���g�̔ԍ���u��������
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// ���񒆂������ꍇ�A���̏���|�C���g���w�肷��
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// �o�H�T���p�̃m�[�h���W���X�V
			mpNavNode->SetPos(Position());

			// ����|�C���g�̌o�H�T���m�[�h�̈ʒu��ݒ肵�Ȃ������ƂŁA
			// �e�m�[�h�ւ̐ڑ������X�V
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}
			// ����|�C���g�܂ł̍ŒZ�o�H�����߂�
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute))
			{
				// ���̖ړI�n�̃C���f�b�N�X��ݒ�
				mNextMoveIndex = 1;
			}
		}
	}
}

// �ҋ@��Ԃ̍X�V����
void CGhost::UpdateIdle()
{
	//// �v���C���[������͈͊O�ɓ�������A�ǐՂɂ���
	//if (IsFoundPlayer())
	//{
	//	ChangeState((int)EState::eChase);
	//	return;
	//}

	// �ʏ펞�̑ҋ@
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState((int)EState::ePatrol);
	}
}

// �p�j��Ԃ̍X�V����
void CGhost::UpdateePatrol()
{
	//if (IsFoundPlayer())
	//{
	//	ChangeState((int)EState::eChase);
	//	return;
	//}

	ChangeAnimation((int)EAnimType::eIdle);

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �����w���̏���|�C���g�����߂�
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
		// �X�e�b�v1 : ����|�C���g�܂ňړ�
	case 1:
	{
		ChangeAnimation((int)EAnimType::eIdle);
		// �ŒZ�o�H�̎��̃m�[�h�܂ňړ�
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];
		if (MoveTo(moveNode->GetPos(), WALK_SPEED))
		{
			// �ړ����I���΁A���̃m�[�h�܂ňړ�
			mNextMoveIndex++;
			// �Ō�̃m�[�h(�ړI�n�̃m�[�h)�������ꍇ�́A���̃X�e�b�v�֐i�߂�
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// �X�e�b�v2 : �ړ���̑ҋ@
	case 2:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;

			if (!mIsTrap)
			{
				// 㩂𐶐�
				CTrap* trap = new CTrap();
				trap->Position(Position() + CVector(0.0f, 1.0f, 0.0f));
			}
		}
		break;
	}
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
