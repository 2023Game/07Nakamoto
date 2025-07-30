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
#include "CSceneManager.h"
#include "CRoom.h"

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

#define FOV_ANGLE			 50.0f	// ����͈͂̊p�x
#define FOV_LENGTH			200.0f	// ����͈͂̋���
#define FOV_HEIGHT			 10.0f	// ����͈͂̍���
#define EYE_HEIGHT			 10.0f	// ���_�̍���

#define ALERT_STOP_TIME		  2.0f	// �x����Ԃ֑J�ڂ���Î~����
#define ALERT_FOV_ANGLE		 60.0f	// �x�����̎���͈͂̊p�x
#define ALERT_FOV_LENGTH	300.0f	// �x�����̎���͈͂̋���

#define PATROL_INTERVAL		  3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST	 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME			  5.0f	// �ҋ@��Ԃ̎���

#define SEARCH_RADIUS	 10.0f		// �󂹂�I�u�W�F�N�g��T�m����͈͂̔��a

#define UPDATE_LOST_POS_TIME 2.0f	// ���������ʒu���X�V���鎞��

// �{�X�̃C���X�^���X
CBoss* CBoss::spInstance = nullptr;

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",							true,	60.0f,	1.0f	},	// �ҋ@		
	{ ANIM_PATH"warrok_walk.x",		true,	44.0f,	1.0f	},	// ���s
	{ ANIM_PATH"warrok_run.x",		true,	27.0f,	1.0f	},	// ���s
	{ ANIM_PATH"warrok_attack.x",	false,	81.0f,	1.0f	},	// �U��
	{ ANIM_PATH"warrok_alert.x",	false,	150.0f,	0.5f	},	// �x��
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
	, mpBattleTargetChara(nullptr)
	, mpBattleTargetObj(nullptr)
	, mpAttack1Col(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
	, mPower(ATTACK_POWER)
	, mDemonPower(0)
	, mStopElapsedTime(0.0f)
#if _DEBUG
	, mpDebugFov(nullptr)
#endif
{
	spInstance = this;

	CPlayer2* target = CPlayer2::Instance();
	mTargetCharas.push_back(target);

	CCat* cat = CCat::Instance();
	mTargetCharas.push_back(cat);

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
		this, ELayer::eBreakableSearch,
		SEARCH_RADIUS
	);
	// �󂹂�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eDoor });

#if _DEBUG
	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

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
	mDemonPower = CDemonPowerManager::Instance()->GetDemonPower();
	//mMaxDemonPower = CDemonPowerManager::Instance()->GetDemonPower();
	//mDemonPower = mMaxDemonPower;

	// ���̍s����擾
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_Head");
	mpHeadMtx = &frame->CombinedMatrix();

}

// �f�X�g���N�^
CBoss::~CBoss()
{
	// �̂̃R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);
	// �U���R���C�_�[���폜
	SAFE_DELETE(mpAttack1Col);
	// �󂹂�I�u�W�F�N�g��T�m����R���C�_�[���폜
	SAFE_DELETE(mpSearchCol);

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
		mpNavNode->Kill();
		mpLostPlayerNode->Kill();

		// ����m�[�h�ɔz�u�����m�[�h���S�č폜
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPoints.erase(itr);
			del->Kill();
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

		// �_���[�W��^�������肪�L�����N�^�[���ǂ�������
		// TODO�F��ѓ�������ꍇ�́A���̎�����𔻒�
		CCharaBase* chara = dynamic_cast<CCharaBase*>(causer);
		if (chara != nullptr)
		{
			// �U���������������퓬����ɐݒ�
			mpBattleTargetChara = chara;

			// �U��������������̕����֌���
			LookAtBattleTarget(true);

			// �퓬��Ԃ֐؂�ւ�
			mIsBattle = true;
		}

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
		CObjectBase* obj = other->Owner();
		if (obj != nullptr)
		{
			// �󂹂�I�u�W�F�N�g�̍폜�t���O�������Ă��Ȃ����A
			// ���Ă����ԂłȂ���΁A
			if (!obj->IsKill() && !obj->IsDeath())
			{
				// �Փ˂����󂹂�I�u�W�F�N�g�����X�g�ɒǉ�
				mNearBreakObjs.push_back(obj);
			}
		}
	}
}

// �퓬����̕��֌���
void CBoss::LookAtBattleTarget(bool immediate)
{
	CObjectBase* battleTarget = GetBattleTarget();
	// �퓬���肪���Ȃ���΁A�������Ȃ�
	if (battleTarget == nullptr) return;

	// �퓬����܂ł̕����x�N�g�������߂�
	CVector targetPos = battleTarget->Position();
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

// ���̐��ʕ����x�N�g�����擾
CVector CBoss::GetHeadForwardVec() const
{
	if (mpHeadMtx == nullptr) return VectorZ();
	
	CMatrix m;
	m.RotateX(-90.0f);
	m = m * (*mpHeadMtx);
	CVector vec = m.VectorZ();
	vec.Y(0.0f);

	CVector start = m.Position();
	CVector end = start + m.VectorZ() * 100.0f;

	// �m�F�p
	//Primitive::DrawLine(start, end, CColor::red, 2.0f);

	return vec.Normalized();
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
	mStopElapsedTime = 0.0f;
}

// ���݂̐퓬������擾
CObjectBase* CBoss::GetBattleTarget() const
{
	// �󂷃I�u�W�F�N�g��D�悵�ĕԂ�
	if (mpBattleTargetObj != nullptr)return mpBattleTargetObj;
	// �󂷃I�u�W�F�N�g��������΁A�퓬����̃L�����N�^�[��Ԃ�
	return mpBattleTargetChara;
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
	// ���g�̓��̐��ʕ����x�N�g�����擾
	CVector forward = GetHeadForwardVec();
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
	CVector targetPos = target->Position();
	CVector selfPos = Position();
	// �^�[�Q�b�g�̍����Ŏ���͈͓�������
	float diffY = abs(targetPos.Y() - selfPos.Y());
	if (diffY >= FOV_HEIGHT) return false;

	// �t�B�[���h�����݂��Ȃ��ꍇ�́A�Օ������Ȃ��̂Ō�����
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// �^�[�Q�b�g�̍��W���擾
	targetPos += offsetPos;
	// �������g�̍��W���擾
	selfPos += offsetPos;

	CHitInfo hit;
	//�t�B�[���h�ƃ��C������s���A�Օ��������݂����ꍇ�́A�^�[�Q�b�g�������Ȃ�
	if (field->CollisionRay(selfPos, targetPos, &hit)) return false;	

	// �^�[�Q�b�g�Ƃ̊ԂɎՕ������Ȃ��̂ŁA�^�[�Q�b�g�������Ă���
	return true;

}

// �^�[�Q�b�g�ɍU���ł��邩�ǂ���
bool CBoss::CanAttackBattleTarget() const
{
	CObjectBase* battleTarget = GetBattleTarget();
	// �^�[�Q�b�g�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	if ((battleTarget == nullptr)) return false;

	// �^�[�Q�b�g�܂ł̋������U���͈͊O�ł���΁A�U���ł��Ȃ�
	CVector targetPos = battleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);	//�������l�����Ȃ�

	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// ���ׂĂ̏������N���A�����̂ŁA�U���ł���
	return true;
}

// �󂹂�I�u�W�F�N�g���U�����邩�m�F
bool CBoss::CheckAttackBreakObj()
{
	// �퓬����i�L�����N�^�[�j�����݂��Ȃ���΁A�󂹂�I�u�W�F�N�g���U�����Ȃ�
	if (mpBattleTargetChara == nullptr) return false;

	// �߂��ɉ󂹂�I�u�W�F�N�g���Ȃ���΁A�󂹂�I�u�W�F�N�g���U�����Ȃ�
	CObjectBase* obj = GetNearBreakObj();
	if (obj == nullptr) return false;

	// �󂹂�I�u�W�F�N�g���U���Ώۂɂ��āA
	// �U����Ԃֈڍs
	mpBattleTargetObj = obj;
	ChangeState((int)EState::eAttack);

	// �U����Ԃֈڍs�����̂ŁAtrue��Ԃ�
	return true;
}

// �L�����N�^�[���U�����邩�m�F
bool CBoss::CheckAttackChara()
{
	for (CCharaBase* target : mTargetCharas)
	{
		// �^�[�Q�b�g�����S���Ă�����A�ǐՑΏۂƂ��Ȃ�
		if (target->IsDeath()) continue;
		// �^�[�Q�b�g������͈͓��ɓ�������A�ǐՂɂ���
		if (IsFoundTarget(target))
		{
			// �퓬����i�L�����N�^�[�j��ݒ�
			mpBattleTargetChara = target;
			ChangeState((int)EState::eChase);
			return true;
		}
	}

	return false;
}

// �퓬����̎��S�m�F�����āA�퓬���肪�܂����݂��邩�m�F
CObjectBase* CBoss::ChackDeathBattleTargets()
{
	// �퓬����i�I�u�W�F�N�g�j��D��Ŏ��S�m�F���s��
	if (mpBattleTargetObj != nullptr)
	{
		// ���S���Ă��Ȃ���΁A�I�u�W�F�N�g��Ԃ�
		if (!mpBattleTargetObj->IsDeath()) return mpBattleTargetObj;

		mpBattleTargetObj = nullptr;

	}
	// �퓬����i�L�����N�^�[�j�̎��S�m�F
	if (mpBattleTargetChara != nullptr)
	{
		// ���S���Ă��Ȃ���΁A�L�����N�^�[��Ԃ�
		if (!mpBattleTargetChara->IsDeath()) return mpBattleTargetChara;
		mpBattleTargetChara = nullptr;
	}
	
	// �퓬���肪�ǂ�������݂��Ȃ������̂ŁAnullptr��Ԃ�
	return nullptr;
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
	// �c��̋������L�����N�^�[�̔��a���Z���ꍇ
	else if (remainDist <= BODY_RADIUS)
	{
		// �����ňړ����I���Ƃ���
		return true;
	}

	// �c��̋������ړ�������蒷���ꍇ�́A
	// �ړ��������ړI�n�ֈړ�����
	pos += moveDir * moveDist;
	Position(pos);

	// �ړI�n�ɂ͓������Ȃ�����
	return false;
}

// ���ɏ��񂷂�|�C���g��ύX����
bool CBoss::ChangePatrolPoint()
{
	// ���g�̌o�H�T���m�[�h���X�V���̏ꍇ�͏������Ȃ�
	if (mpNavNode->IsUpdating()) return false;
	// ����|�C���g���ݒ肳��Ă��Ȃ��ꍇ�͏������Ȃ�
	int size = mPatrolPoints.size();
	if (size == 0) return false;

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

	return mNextPatrolIndex >= 0;
}

// ���񃋁[�g���X�V����
bool CBoss::UpdatePatrolRouto()
{
	// ����|�C���g�̌o�H�T���m�[�h�̈ʒu��ݒ肵�Ȃ������ƂŁA
	// �e�m�[�h�ւ̐ڑ������X�V
	for (CNavNode* node : mPatrolPoints)
	{
		node->SetPos(node->GetPos());
	}

	if (!(0 <= mNextPatrolIndex && mNextPatrolIndex < mPatrolPoints.size())) return false;

	CNavManager* navMgr = CNavManager::Instance();
	if(navMgr == nullptr) return false;

	// ���g�̃m�[�h���X�V���Ȃ�΁A�o�H�T���͍s��Ȃ�
	if (mpNavNode->IsUpdating()) return false;
	// ����|�C���g���X�V���Ȃ�΁A�o�H�T�����s��
	CNavNode* patrolPoint = mPatrolPoints[mNextPatrolIndex];
	if (patrolPoint->IsUpdating()) return false;
	
	// ����|�C���g�܂ł̍ŒZ�o�H�����߂�
	if (navMgr->Navigate(mpNavNode, patrolPoint, mMoveRoute))
	{
		// ���̖ړI�n�̃C���f�b�N�X��ݒ�
		mNextMoveIndex = 1;
	}
}

// �ҋ@��Ԃ̍X�V����
void CBoss::UpdateIdle()
{
	// �U������L�����N�^�[�����������ꍇ�́A���̏��������s���Ȃ�
	if (CheckAttackChara())
	{
		return;
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
	// �U������L�����N�^�[�����������ꍇ�́A���̏��������s���Ȃ�
	if (CheckAttackChara())
	{
		return;
	}

	ChangeAnimation((int)EAnimType::eWalk);

	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
	// �X�e�b�v0 : ����J�n�̏���|�C���g�����߂�
	case 0:
		mNextPatrolIndex = -1;
		if (ChangePatrolPoint())
		{
			mStateStep++;
		}
		
		break;
	// �X�e�b�v1 : ����|�C���g�܂ł̈ړ��o�H�T��
	case 1:
		if(UpdatePatrolRouto())
		{
			mStateStep++;
		}
		break;
	// �X�e�b�v2 : ����|�C���g�܂ňړ�
	case 2:
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
	// �X�e�b�v3 : �ړ���̑ҋ@
	case 3:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			if (ChangePatrolPoint())
			{
				mStateStep = 1;
				mElapsedTime = 0.0f;
			}
		}
		break;
	}
}

// �ǂ������鎞�̍X�V����
void CBoss::UpdateChase()
{
	// �퓬���肪���݂��Ȃ������ꍇ�́A
	CObjectBase* battleTarget = ChackDeathBattleTargets();
	if (battleTarget == nullptr)
	{
		// �^�[�Q�b�g���������A�ҋ@��Ԃ֖߂�
		ChangeState((int)EState::eIdle);
		return;
	}

	// ��莞�ԐÎ~����ƁA�x����Ԃֈڍs
	if (mStopElapsedTime >= ALERT_STOP_TIME)
	{
		// �ړ����I���Όx����Ԃֈڍs
		ChangeState((int)EState::eAlert);
		return;
	}

	// �^�[�Q�b�g�̍��W�֌����Ĉړ�����
	CVector targetPos = battleTarget->Position();

	// �^�[�Q�b�g�������Ȃ��Ȃ�����A����������Ԃɂ���
	if (!IsLookTarget(battleTarget))
	{
		// ���������ʒu�Ƀm�[�h��z�u
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState((int)EState::eLost);
		return;
	}

	// �^�[�Q�b�g�ɍU���ł���Ȃ�΁A
	if (CanAttackBattleTarget())
	{
		// �^�[�Q�b�g�̍����̍���������͈͓��Ȃ��
		float diffY = abs(battleTarget->Position().Y() - Position().Y());
		if (diffY < FOV_HEIGHT)
		{
			// �U����Ԃֈڍs
			ChangeState((int)EState::eAttack);
		}
		// ����͈͊O�ł���΁A�x����Ԃֈڍs
		else
		{
			ChangeState((int)EState::eAlert);
		}
		return;
	}
 
	// �߂��̉󂹂�I�u�W�F�N�g���󂷂̂ł���ΒǐՏ�Ԃ𔲂���
	if (CheckAttackBreakObj())
	{
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	// �o�H�T���Ǘ��N���X�����݂���΁A
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ���g�̃m�[�h�ƃ^�[�Q�b�g�̃m�[�h���X�V���łȂ���΁A
		// ���g�̃m�[�h����^�[�Q�b�g�̃m�[�h�܂ł̍ŒZ�o�H�����߂�
		CNavNode* targetNode = battleTarget->GetNavNode();
		if (!mpNavNode->IsUpdating()
			&& targetNode != nullptr && !targetNode->IsUpdating())
		{
			if (navMgr->Navigate(mpNavNode, targetNode, mMoveRoute))
			{
				// ���g�̃m�[�h����^�[�Q�b�g�̃m�[�h�܂Ōq�����Ă�����A
				// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
				targetPos = mMoveRoute[1]->GetPos();
			}
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

	// �퓬���肪���݂��Ȃ������ꍇ�́A
	CObjectBase* battleTarget = ChackDeathBattleTargets();
	if (battleTarget == nullptr)
	{
		// �^�[�Q�b�g���������A�ҋ@��Ԃ֖߂�
		ChangeState((int)EState::eIdle);
		return;
	}

	// �^�[�Q�b�g����������A�ǐՏ�Ԃֈڍs
	if (IsLookTarget(battleTarget))
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// �߂��̉󂹂�I�u�W�F�N�g���󂷂̂ł���΁A����������Ԃ𔲂���
	if (CheckAttackBreakObj())
	{
		return;
	}

	// ��莞�ԐÎ~����ƁA�x����Ԃֈڍs
	if (mStopElapsedTime >= ALERT_STOP_TIME)
	{
		// �ړ����I���Όx����Ԃֈڍs
		ChangeState((int)EState::eAlert);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eRun);

	switch (mStateStep)
	{
			// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
		case 0:
			mLostElapsedTime = 0.0f;
			mStateStep++;
			break;
		case 1:
			// �o�H�T���p�̃m�[�h���W���X�V
			mpLostPlayerNode->SetPos(battleTarget->Position());

			// ���g�̃m�[�h�ƌ��������ʒu�̃m�[�h���X�V���łȂ����
			if (!mpNavNode->IsUpdating() && !mpLostPlayerNode->IsUpdating())
			{
				if (!navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
				{
					// �o�H���Ȃ����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
					ChangeState((int)EState::eIdle);
					mpLostPlayerNode->SetEnable(false);
				}
				else
				{
					// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
					// ���̈ړ�����P�ԋ߂��ꏊ�ɐݒ�
					mNextMoveIndex = 1;
				}
			}
		case 2:
			// ���̈ړ���̃C���f�b�N�X�l���s���l�łȂ����
			if (0 <= mNextMoveIndex && mNextMoveIndex < mMoveRoute.size())
			{
				// �^�[�Q�b�g�����������ʒu�܂ňړ�
				if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
				{
					mNextMoveIndex++;
					if (mNextMoveIndex >= mMoveRoute.size())
					{
						// �ړ����I���Όx����Ԃֈڍs
						ChangeState((int)EState::eAlert);
						mpLostPlayerNode->SetEnable(false);
					}
				}

				if (mLostElapsedTime < UPDATE_LOST_POS_TIME)
				{
					// �����������Ԃ����Z
					mLostElapsedTime += Times::DeltaTime();
					if (mLostElapsedTime >= UPDATE_LOST_POS_TIME)
					{
						mStateStep++;
					}
				}
			}
			break;
	}
}

// �x�����Ă��鎞�̏���
void CBoss::UpdateAlert()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
	// �X�e�b�v�P�F�x���A�j���[�V�����Đ�
	case 0:
		ChangeAnimation((int)EAnimType::eAlert);
		mStateStep++;
		break;
	// �X�e�b�v�Q�F�x���J�n
	case 1:
		// �U���ł���L�����N�^�[�����݂����ꍇ�́A�����ŏI��
		if (CheckAttackChara())
		{
			return;
		}

		// �A�j���[�V�������I�������
		if (IsAnimationFinished())
		{
			// �ҋ@k��Ԃֈڍs
			ChangeState((int)EState::eIdle());
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
			mStateStep++;
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
			// �퓬����̎��S�`�F�b�N
			CObjectBase* batteleTarget = ChackDeathBattleTargets();
			// �퓬���肪���݂��Ȃ������ꍇ
			if (batteleTarget == nullptr)
			{
				// �ҋ@��Ԃֈڍs
				ChangeState((int)EState::eIdle);
			}
			// �퓬���肪���݂����ꍇ
			else
			{
				// �퓬���肪����͈͓��ɂ����ꍇ�́A�ǐՏ�Ԃֈڍs
				if (IsLookTarget(batteleTarget))
				{
					ChangeState((int)EState::eChase);
				}
				// �퓬���肪����͈͓��ɂ��Ȃ������ꍇ�A����������Ԃֈڍs
				else
				{
					mpLostPlayerNode->SetPos(batteleTarget->Position());
					mpLostPlayerNode->SetEnable(true);
					ChangeState((int)EState::eLost);
				}
			}
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
	// �V�[�����N���A�V�[���ɕύX
	CSceneManager::Instance()->LoadScene(EScene::eClear);
}

// �X�V
void CBoss::Update()
{
	CVector currPos = Position();
	float moveDist = (currPos - mLastPos).Length();
	if (moveDist <= 0.1f)
	{
		mStopElapsedTime += Times::DeltaTime();
	}
	else
	{
		mStopElapsedTime = 0.0f;
	}
	mLastPos = Position();

#if _DEBUG
	CDebugPrint::Print("moveDist:%f\n", moveDist);
	CDebugPrint::Print("StopTime:%f\n", mStopElapsedTime);
#endif

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
	else if (demonPower == 0)
	{
		ChangeState((int)EState::eDeath);
	}

	// �x�����͎���͈͂��L����
	if (mState == (int)EState::eAlert)
	{
		mFovAngle = ALERT_FOV_ANGLE;
		mFovLength = ALERT_FOV_LENGTH;
	}
	// �ʏ펞�̎���͈͂�ݒ�
	else
	{
		mFovAngle = FOV_ANGLE;
		mFovLength = FOV_LENGTH;
	}

#if _DEBUG
	mpDebugFov->Set(mFovAngle, mFovLength);
#endif

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
	// �x�����
	case EState::eAlert:	UpdateAlert();	break;
	// ���S���
	case EState::eDeath:	UpdateDeath();	break;
	}

	// �G�̃x�[�X�N���X�̍X�V
	CEnemy::Update();

	// �o�H�T���p�̃m�[�h���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// �󂹂�I�u�W�F�N�g�̃��X�g���N���A
	mNearBreakObjs.clear();

#if _DEBUG
	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor((EState)mState));

	CDebugPrint::Print("��� : %s\n", GetStateStr((EState)mState).c_str());
	CDebugPrint::Print("����(�{�X)�F%s\n", mpRoom != nullptr ? mpRoom->GetName().c_str() : "�Ȃ�");


#endif
}

// �`��
void CBoss::Render()
{
	CEnemy::Render();

#if _DEBUG

	// ����͈͂̃f�o�b�O�\���ɓ��̌����𔽉f
	CVector vec = GetHeadForwardVec();
	mHeadForwardMtx = CMatrix::LookRotation(vec, CVector::up);
	mHeadForwardMtx.Position(Position());
	mpDebugFov->SetMatrix(&mHeadForwardMtx);

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

	CObjectBase* battleTarget = GetBattleTarget();

	if (battleTarget != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector targetPos = battleTarget->Position() + offsetPos;
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
CObjectBase* CBoss::GetNearBreakObj() const
{
	// �L�����N�^�[��ǂ������Ă����ԂłȂ���΁A�󂹂�I�u�W�F�N�g���󂳂Ȃ�
	if (mpBattleTargetChara == nullptr) return nullptr;
	// ���ɉ󂹂�I�u�W�F�N�g���U�����ł���΁A�V�����󂹂�I�u�W�F�N�g��ݒ肵�Ȃ�
	if (mpBattleTargetObj != nullptr) return nullptr;

	// ��ԋ߂��̉󂹂�I�u�W�F�N�g�̃|�C���^�i�[�p
	CObjectBase* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ���󂹂�I�u�W�F�N�g�܂ł̋���
	CVector pos = Position();	// �G���g�̍��W���擾
	// �T�m�͈͓��̉󂹂�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CObjectBase* obj : mNearBreakObjs)
	{
		// �󂹂�I�u�W�F�N�g���h�A���ǂ����m�F
		CDoorBase* door = dynamic_cast<CDoorBase*>(obj);
		if (door != nullptr)
		{
			// �v���C���[�������Ă��镔�����擾
			CRoom* playerRoom = mpBattleTargetChara->GetRoom();
			// �v���C���[�Ǝ��g�����������ɂ���ꍇ�́A�h�A���󂳂Ȃ�
			if (playerRoom == mpRoom) continue;

			// �v���C���[�����g�����镔���̃h�A�łȂ���΁A�X���[
			CRoom* doorRoom = door->GetRoom();
			if (playerRoom != doorRoom && mpRoom != doorRoom) continue;
		}
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
		// ��ԍŏ��̉󂹂�I�u�W�F�N�g���A
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