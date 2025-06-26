#include "CBoss.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CCactusNeedle.h"

#include "CPlayer2.h"
#include "CCat.h"
#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "CField.h"
#include "CDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CInteractObject.h"
#include "CDemonPowerManager.h"

#if _DEBUG
#include "CSceneManager.h"
#include "CTestField.h"
#endif

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Enemy\\Warrok\\anim\\"

#define BODY_HEIGHT 13.0f	// �J�v�Z���R���C�_�[�̏�̍���
#define BODY_WIDTH	5.0f	// �J�v�Z���R���C�_�[�̕�
#define BODY_RADIUS 4.5f	// �J�v�Z���R���C�_�[�̔��a

#define WALK_SPEED		10.0f	// �������x
#define RUN_SPEED		20.0f	// ���鑬�x
#define ROTATE_SPEED	6.0f	// ��]���x

#define LOOKAT_SPEED 90.0f	// ����̕��֌�������
#define ATTACK_RANGE 15.0f	// �U��������͈�
#define ATTACK_WAIT_TIME	  1.0f	// �U���I�����̑҂�����

#define ATTACK_COL_RADIUS 10.0f	// �U���R���C�_�[�̔��a
#define ATTACK_COL_POS CVector(0.0f, 5.0f, 8.0f)	// �U���R���C�_�[�̈ʒu

#define ATTACK_POWER 20		// �U����
#define ATTACK_START_FRAME 39.0f	// �U������J�n�t���[��
#define ATTACK_END_FRAME 46.0f		// �U������I���t���[��

#define FOV_ANGLE			 45.0f	// ����͈͂̊p�x
#define FOV_LENGTH			100.0f	// ����͈͂̋���
#define EYE_HEIGHT			 10.0f	// ���_�̍���

#define PATROL_INTERVAL		  3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST	 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME			  5.0f	// �ҋ@��Ԃ̎���

#define SEARCH_RADIUS	 10.0f		// �󂹂�I�u�W�F�N�g��T�m����͈͂̔��a

// �{�X�̃C���X�^���X
CBoss* CBoss::spInstance = nullptr;

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",							true,	60.0f,	1.0f	},	// �ҋ@		
	{ ANIM_PATH"warrok_walk.x",		true,	44.0f,	1.0f	},	// ���s
	{ ANIM_PATH"warrok_run.x",		true,	27.0f,	1.0f	},	// ���s
	{ ANIM_PATH"warrok_attack.x",	false,	81.0f,	1.0f	},	// �U��

};

// �C���X�^���X�̃|�C���^���擾
CBoss* CBoss::Instance()
{
	return spInstance;
}

// �R���X�g���N�^
CBoss::CBoss(std::vector<CVector> patrolPoints)
	: mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mpAttack1Col(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
	, mPower(ATTACK_POWER)
	, mDemonPower(0)
{
	spInstance = this;

	CPlayer2* target = CPlayer2::Instance();
	mTargets.push_back(target);

	CCat* cat = CCat::Instance();
	mTargets.push_back(cat);

	// �G��������
	InitEnemy("Warrok", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// �̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_WIDTH, 0.0f),
		CVector(0.0f, BODY_HEIGHT, 0.0f),
		BODY_RADIUS, true
	);

	// �t�B�[���h�ƁA�^�[�Q�b�g�̍U���R���C�_�[�ƃq�b�g����悤�ɐݒ�
	mpBodyCol->SetCollisionTags
	(
		{ 
			ETag::eField,
			ETag::eGimmick,
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{	ELayer::eFloor,
			ELayer::eWall,
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractObj,
			ELayer::eDoor
		}
	);

	// �U���R���C�_�[���쐬
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// �^�[�Q�b�g�̖{�̃R���C�_�[�Ƃ̂݃q�b�g����悤�ɐݒ�
	mpAttack1Col->SetCollisionTags
	(
		{ 
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject,
		}
	);
	mpAttack1Col->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eDoor,
		}
	);
	// �U���R���C�_�[�̍��W��ݒ�
	mpAttack1Col->Position(ATTACK_COL_POS);
	// �U���R���C�_�[���ŏ��̓I�t�ɂ��Ă���
	mpAttack1Col->SetEnable(false);

	// �󂹂�I�u�W�F�N�g��T�m����R���C�_�[���쐬
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	// �󂹂�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eDoor });

#if _DEBUG
	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

	mScene = CSceneManager::Instance()->GetCurrentScene();
#endif

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// �^�[�Q�b�g�����������ʒu�̃m�[�h���쐬
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);

	// ����|�C���g�Ɍo�H�T���p�̃m�[�h��z�u
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

	// �d�͂̌��̐����擾
	//mMaxDemonPower = CDemonPowerManager::Instance()->GetDemonPower();
	//mDemonPower = mMaxDemonPower;
}

// �f�X�g���N�^
CBoss::~CBoss()
{
	// �̂̃R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);
	// �U���R���C�_�[���폜
	SAFE_DELETE(mpAttack1Col);

	// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜����
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpLostPlayerNode);

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

// �I�u�W�F�N�g�폜��`����֐�
void CBoss::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���ł����
	// ���g���폜���ꂽ
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// �U������
bool CBoss::IsAttacking() const
{
	// �p���`�U��
	if (mState == (int)EState::eAttack) return true;

	// �U�����łȂ�
	return false;
}

// �U���J�n
void CBoss::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CEnemy::AttackStart();

	// �p���`�U�����ł���΁A�p���`�U���̃R���C�_�[���I���ɂ���
	if (mState == (int)EState::eAttack)
	{
		mpAttack1Col->SetEnable(true);
	}
}

// �U���I��
void CBoss::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CEnemy::AttackEnd();

	// �U���R���C�_�[���I�t
	mpAttack1Col->SetEnable(false);
}

// �d�͂̌��̌���
void CBoss::PowerDown()
{
	mDemonPower--;
}

// �_���[�W���󂯂�
void CBoss::TakeDamage(int damage, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CEnemy::TakeDamage(damage, causer);

	// ���S���Ă��Ȃ���΁A
	if (!IsDeath())
	{
		// �������Ԃֈڍs
		// ChangeState((int)EState::eHit);

		// �U���������������퓬����ɐݒ�
		mpBattleTarget = causer;

		// �U��������������̕����֌���
		LookAtBattleTarget(true);

		// �퓬��Ԃ֐؂�ւ�
		mIsBattle = true;

		// �ړ����~
		mMoveSpeed = CVector::zero;
	}
}

// ���S
void CBoss::Death()
{
}

// �Փˏ���
void CBoss::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �x�[�X�̏Փˏ������Ăяo��
	CEnemy::Collision(self, other, hit);

	// �U���R���C�_�[���q�b�g����
	if (self == mpAttack1Col)
	{
		// �q�b�g�����̂��L�����N�^�[���A
		// �܂��U�����q�b�g���Ă��Ȃ��L�����N�^�[�ł����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// �_���[�W��^����
			chara->TakeDamage(mPower, this);
			// �U���q�b�g�ς݃��X�g�ɓo�^
			AddAttackHitObj(chara);
		}

		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr && !IsAttackHitObj(obj))
		{
			// �_���[�W��^����
			obj->TakeDamage(mPower, this);
			// �U���q�b�g�ς݃��X�g�ɓo�^
			AddAttackHitObj(obj);
		}
	}
	// �I�u�W�F�N�g�T�[�`�p�̏ꍇ
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// �󂹂�I�u�W�F�N�g�̍폜�t���O�������Ă��Ȃ�������
			if (!obj->IsKill())
			{
				// �Փ˂������ׂ�I�u�W�F�N�g�����X�g�ɒǉ�
				mNearBreakObjs.push_back(obj);
			}
		}
	}
}

// �퓬����̕��֌���
void CBoss::LookAtBattleTarget(bool immediate)
{
	// �퓬���肪���Ȃ���΁A�������Ȃ�
	if (mpBattleTarget == nullptr) return;

	// �퓬����܂ł̕����x�N�g�������߂�
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// �����ɐ퓬����̕����֌���
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// ���X�ɐ퓬����̕����֌���
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// ��Ԑ؂�ւ�
void CBoss::ChangeState(int state)
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

// ���݈ʒu����^�[�Q�b�g�������Ă��邩�ǂ���
bool CBoss::IsFoundTarget(CObjectBase* target) const
{
	// �^�[�Q�b�g���W���擾
	CVector targetPos = target->Position();
	// �������g�̍��W���擾
	CVector pos = Position();
	// ���g����^�[�Q�b�g�܂ł̃x�N�g�������߂�
	CVector  vec = targetPos - pos;
	vec.Y(0.0f);	//�^�[�Q�b�g�Ƃ̍����̍����l�����Ȃ�

	// �@ ����p�x�������߂�
	// �x�N�g���𐳋K�����Ē�����1�ɂ���
	CVector dir = vec.Normalized();
	// ���g�̐��ʕ����x�N�g�����擾
	CVector forward = VectorZ();
	// �^�[�Q�b�g�܂ł̃x�N�g����
	// ���g�̐��ʕ����x�N�g���̓��ς����߂Ċp�x���o��
	float dot = CVector::Dot(dir, forward);
	// ����p�x�̃��W�A�������߂�
	float angleR = Math::DegreeToRadian(mFovAngle);
	// ���߂����ςƎ���p�x�ŁA����͈͓������f����
	if (dot < cosf(angleR))	return false;

	// �A ���싗���������߂�
	//�^�[�Q�b�g�܂ł̋����Ǝ��싗���ŁA����͈͓������f����
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// �^�[�Q�b�g�Ƃ̊ԂɎՕ������Ȃ����`�F�b�N����
	if (!IsLookTarget(target)) return false;

	//�S�Ă̏������N���A�����̂ŁA����͈͓��ł���
	return true;
}

// ���݈ʒu����^�[�Q�b�g�������Ă��邩�ǂ���
bool CBoss::IsLookTarget(CObjectBase* target) const
{
#if _DEBUG
	if (mScene == EScene::eTest)
	{
		CTestField* field = CTestField::Instance();

		if (field == nullptr) return true;

		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		// �^�[�Q�b�g�̍��W���擾
		CVector targetPos = target->Position() + offsetPos;
		// �������g�̍��W���擾
		CVector selfPos = Position() + offsetPos;

		CHitInfo hit;
		//�t�B�[���h�ƃ��C������s���A�Օ��������݂����ꍇ�́A�^�[�Q�b�g�������Ȃ�
		if (field->CollisionRay(selfPos, targetPos, &hit)) return false;

		// �^�[�Q�b�g�Ƃ̊ԂɎՕ������Ȃ��̂ŁA�^�[�Q�b�g�������Ă���
		return true;
	}
#endif

	// �t�B�[���h�����݂��Ȃ��ꍇ�́A�Օ������Ȃ��̂Ō�����
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// �^�[�Q�b�g�̍��W���擾
	CVector targetPos = target->Position() + offsetPos;
	// �������g�̍��W���擾
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	//�t�B�[���h�ƃ��C������s���A�Օ��������݂����ꍇ�́A�^�[�Q�b�g�������Ȃ�
	if (field->CollisionRay(selfPos, targetPos, &hit)) return false;	

	// �^�[�Q�b�g�Ƃ̊ԂɎՕ������Ȃ��̂ŁA�^�[�Q�b�g�������Ă���
	return true;

}

// �^�[�Q�b�g�ɍU���ł��邩�ǂ���
bool CBoss::CanAttackPlayer() const
{
	// �^�[�Q�b�g�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	if ((mpBattleTarget == nullptr)) return false;

	// �^�[�Q�b�g�܂ł̋������U���͈͊O�ł���΁A�U���ł��Ȃ�
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);	//�������l�����Ȃ�

	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// ���ׂĂ̏������N���A�����̂ŁA�U���ł���
	return true;
}

// �w�肵���ʒu�܂ňړ�����
bool CBoss::MoveTo(const CVector& targetPos, float speed)
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
void CBoss::ChangePatrolPoint()
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
void CBoss::UpdateIdle()
{
	// �^�[�Q�b�g�̃I�u�W�F�N�g������΁A
	if (mNearBreakObjs.size())
	{
		ChangeState((int)EState::eChase);
	}

	for (CObjectBase* target : mTargets)
	{
		// �^�[�Q�b�g�����S���Ă�����A�ǐՑΏۂƂ��Ȃ�
		if (target->IsDeath()) continue;
		// �^�[�Q�b�g������͈͓��ɓ�������A�ǐՂɂ���
		if (IsFoundTarget(target))
		{
			mpBattleTarget = target;
			ChangeState((int)EState::eChase);
			return;
		}
	}

	// �ҋ@�A�j���[�V�������Đ�
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

// ���񒆂̍X�V����
void CBoss::UpdatePatrol()
{
	for (CObjectBase* target : mTargets)
	{
		// �^�[�Q�b�g�����S���Ă�����A�ǐՑΏۂƂ��Ȃ�
		if (target->IsDeath()) continue;
		// �^�[�Q�b�g������͈͓��ɓ�������A�ǐՂɂ���
		if (IsFoundTarget(target))
		{
			mpBattleTarget = target;
			ChangeState((int)EState::eChase);
			return;
		}
	}

	ChangeAnimation((int)EAnimType::eWalk);

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
	// �X�e�b�v0 : ����J�n�̏���|�C���g�����߂�
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
	// �X�e�b�v1 : ����|�C���g�܂ňړ�
	case 1:
	{
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
		}
		break;
	}
}

// �ǂ������鎞�̍X�V����
void CBoss::UpdateChase()
{
	// �^�[�Q�b�g�̍��W�֌����Ĉړ�����
	CVector targetPos = mpBattleTarget->Position();
	// �^�[�Q�b�g�����S���Ă�����
	if (mpBattleTarget->IsDeath())
	{
		// �^�[�Q�b�g���������A�ҋ@��Ԃ֖߂�
		mpBattleTarget = nullptr;
		ChangeState((int)EState::eIdle);
		return;
	}

	// �^�[�Q�b�g�������Ȃ��Ȃ�����A����������Ԃɂ���
	if (!IsLookTarget(mpBattleTarget))
	{
		// ���������ʒu�Ƀm�[�h��z�u
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState((int)EState::eLost);
		return;
	}

	// �^�[�Q�b�g�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer())
	{
		ChangeState((int)EState::eAttack);
		return;
	}

	// �߂��̉󂹂�I�u�W�F�N�g���擾
	CInteractObject* obj = GetNearBreakObj();
	if (obj != nullptr)
	{
		mpBattleTarget = obj;
		ChangeState((int)EState::eAttack);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	// �o�H�T���Ǘ��N���X�����݂���΁A
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �o�H�T���p�̃m�[�h���W���X�V
		mpNavNode->SetPos(Position());

		// ���g�̃m�[�h����^�[�Q�b�g�̃m�[�h�܂ł̍ŒZ�o�H�����߂�
		CNavNode* targetNode = mpBattleTarget->GetNavNode();
		if (navMgr->Navigate(mpNavNode, targetNode, mMoveRoute))
		{
			// ���g�̃m�[�h����^�[�Q�b�g�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// �ړ�����
	if (MoveTo(targetPos, RUN_SPEED))
	{

	}
}

// �^�[�Q�b�g�������������̍X�V����
void CBoss::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		mpBattleTarget = nullptr;
		ChangeState((int)EState::eIdle);
		return;
	}

	// �^�[�Q�b�g�����S���Ă�����
	if (mpBattleTarget->IsDeath())
	{
		// �^�[�Q�b�g���������A�ҋ@��Ԃ֖߂�
		mpBattleTarget = nullptr;
		ChangeState((int)EState::eIdle);
		return;
	}

	// �^�[�Q�b�g����������A�ǐՏ�Ԃֈڍs
	if (IsLookTarget(mpBattleTarget))
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �߂��̉󂹂�I�u�W�F�N�g���擾
	CInteractObject* obj = GetNearBreakObj();
	if (obj != nullptr)
	{
		mpBattleTarget = obj;
		ChangeState((int)EState::eAttack);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	switch (mStateStep)
	{
		// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 0:
		// �o�H�T���p�̃m�[�h���W���X�V
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
		{
			// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			//mpBattleTarget = nullptr;
			// �o�H���Ȃ����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
			ChangeState((int)EState::eIdle);
			mpLostPlayerNode->SetEnable(false);
		}
		break;
	case 1:
		// �^�[�Q�b�g�����������ʒu�܂ňړ�
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				//mpBattleTarget = nullptr;
				// �ړ����I���Αҋ@��Ԃֈڍs
				ChangeState((int)EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// �p���`�U�����̍X�V����
void CBoss::UpdateAttack1()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�U���A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eAttack, true);
		mStateStep++;
		break;
		// �X�e�b�v1�F�U���J�n
	case 1:
		// �U�����J�n����܂ŁA���X�ɐ퓬����̕����֌���
		LookAtBattleTarget();

		// �U���J�n�t���[���܂Ōo�߂�����
		if (GetAnimationFrame() >= ATTACK_START_FRAME)
		{
			// �U���J�n�������Ăяo��
			AttackStart();
			mStateStep++;
		}
		break;
		// �X�e�b�v2�F�U���I��
	case 2:
		if (GetAnimationFrame() >= ATTACK_END_FRAME)
		{
			// �U���I���������Ăяo��
			AttackEnd();
			mStateStep++;
		}
		break;
		// �X�e�b�v3�F�U���A�j���[�V�����I���҂�
	case 3:
		// �A�j���[�V�����I��������A�ҋ@��Ԃ֖߂�
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eIdle);
		}
		break;
		// �X�e�b�v�S : �U���I����̑҂�����
	case 4:
		if (mElapsedTime < ATTACK_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

// �j�U�����̍X�V����
void CBoss::UpdateAttack2()
{
}

// �������Ԃ̍X�V����
void CBoss::UpdateHit()
{
}

// ���S��Ԃ̍X�V����
void CBoss::UpdateDeath()
{
}

// �X�V
void CBoss::Update()
{
	// �d�͂̌��̐����擾
	int demonPower = CDemonPowerManager::Instance()->GetDemonPower();
	// �d�͂̌����c��3�̎��A
	if (demonPower == 3)
	{
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}
	// �d�͂̌����c��3�̎��A
	else if (demonPower == 2)
	{
		mPower = ATTACK_POWER + ATTACK_POWER;
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}
	// �d�͂̌����c��3�̎��A
	else if (demonPower == 1)
	{
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch ((EState)mState)
	{
		// �ҋ@���
	case EState::eIdle:		UpdateIdle();	break;
		// ������
	case EState::ePatrol:	UpdatePatrol();	break;
		// �ǂ�������
	case EState::eChase:	UpdateChase();	break;
		// �����������
	case EState::eLost:		UpdateLost();	break;
		// �p���`�U��
	case EState::eAttack:	UpdateAttack1(); break;
		//	// �j�U��
		//case EState::eAttack2:	UpdateAttack2(); break;
		//	// ������
		//case EState::eHit:		UpdateHit();	break;
		//	// ���S���
		//case EState::eDeath:	UpdateDeath();	break;
	}

	// �G�̃x�[�X�N���X�̍X�V
	CEnemy::Update();

	// �󂹂�I�u�W�F�N�g�̃��X�g���N���A
	mNearBreakObjs.clear();

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor((EState)mState));

	CDebugPrint::Print("��� : %s\n", GetStateStr((EState)mState).c_str());
#endif
}

// �`��
void CBoss::Render()
{
	CEnemy::Render();

#if _DEBUG
	// ���񒆂ł���΁A
	if (mState == (int)EState::ePatrol)
	{
		float rad = 1.0f;
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawBox
			(
				mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f),
				CVector::one * rad,
				c
			);
		}
	}
	// ����������Ԃł���΁A
	if (mState == (int)EState::eLost)
	{
		//�^�[�Q�b�g�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		Primitive::DrawBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f),
			CVector::one * rad,
			CColor::blue
		);
	}

	CField* field = CField::Instance();
	if (mpBattleTarget != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector targetPos = mpBattleTarget->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		//�^�[�Q�b�g�Ƃ̊ԂɎՕ��������݂���ꍇ
		CHitInfo hit;
		if (field->CollisionRay(selfPos, targetPos, &hit))
		{
			// �Փ˂����ʒu�܂ŐԐ���`��
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// �Օ��������݂��Ȃ������ꍇ
		else
		{
			// �^�[�Q�b�g�̈ʒu�܂ŗΐ���`��
			Primitive::DrawLine
			(
				selfPos, targetPos,
				CColor::green,
				2.0f
			);
		}
	}
#endif
}

// ��Ԃ̕�������擾
std::string CBoss::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:		return "�ҋ@";
	case EState::ePatrol:	return "����";
	case EState::eChase:	return "�ǐ�";
	case EState::eLost:		return "������";
	case EState::eAttack:	return "�U��";
	}
	return "";
}

// ��Ԃ̐F���擾
CColor CBoss::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:		return CColor::white;
	case EState::ePatrol:	return CColor::green;
	case EState::eChase:	return CColor::red;
	case EState::eLost:		return CColor::yellow;
	case EState::eAttack:	return CColor::magenta;
	}
	return CColor::white;
}

// ��ԋ߂��ɂ���󂹂�I�u�W�F�N�g���擾
CInteractObject* CBoss::GetNearBreakObj() const
{
	// ��ԋ߂��̒��ׂ�I�u�W�F�N�g�̃|�C���^�i�[�p
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g�܂ł̋���
	CVector pos = Position();	// �G���g�̍��W���擾
	// �T�m�͈͓��̒��ׂ�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CInteractObject* obj : mNearBreakObjs)
	{
		// ���ݒ��ׂ��Ȃ���Ԃł���΁A�X���[
		if (!obj->CanInteract()) continue;

		// �I�u�W�F�N�g�̍��W���擾
		CVector objPos = obj->Position();
		// �^�[�Q�b�g����I�u�W�F�N�g�܂ł̃x�N�g�������߂�
		CVector vec = objPos - pos;
		vec.Y(0.0f);	// �I�u�W�F�N�g�Ƃ̍����̍����l�����Ȃ�

		// �x�N�g���𐳋K�����Ē�����1�ɂ���
		CVector dir = vec.Normalized();
		// ���g�̐��ʕ����x�N�g�����擾
		CVector forward = VectorZ();
		// �^�[�Q�b�g�Ƃ܂ł̃x�N�g����
		// ���g�̐��ʕ����x�N�g���̓��ς����߂Ċp�x���o��
		float dot = CVector::Dot(dir, forward);
		// ����p�x�̃��W�A�������߂�
		float angleR = Math::DegreeToRadian(mFovAngle);
		// ���߂����ςƎ���p�x�ŁA����͈͓������f����
		if (dot < cosf(angleR)) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// ��ԍŏ��̒��ׂ�I�u�W�F�N�g���A
		// ���߂����������݂̈�ԋ߂��I�u�W�F�N�g�����߂��ꍇ�́A
		if (nearObj == nullptr || dist < nearDist)
		{
			// ��ԋ߂��I�u�W�F�N�g���X�V
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}