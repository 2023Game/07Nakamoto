#include "CEnemy.h"
#include "CEffect.h"
#include "CInput.h"
#include "CCollisionManager.h"
#include "CDebugFieldOfView.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "Primitive.h"
#include "CField2.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CTrap.h"

#define ENEMY_HEIGHT		 16.0f	// �G�̍���
#define ENEMY_WIDTH			 10.0f	// �G�̕�
#define FOV_ANGLE			 45.0f	// ����͈͂̊p�x
#define FOV_LENGTH			100.0f	// ����͈͂̋���
#define EYE_HEIGHT			 10.0f	// ���_�̍���
#define WALK_SPEED			 10.0f	// �����Ă���Ƃ��̑��x
#define RUN_SPEED			 20.0f	// �����Ă�����Ƃ��̑��x
#define ROTATE_SPEED		 6.0f	// ��]���x

#define ATTACK_RANGE		 20.0f	// �U���͈�
#define ATTACK_MOVE_DIST	 20.0f	// �U�����̈ړ�����
#define ATTACK_MOVE_STAT	 16.0f	// �U�����̈ړ��J�n�t���[��
#define ATTACK_MOVE_END		 47.0f	// �U�����̈ړ��I���t���[��
#define ATTACK_WAIT_TIME	  1.0f	// �U���I�����̑҂�����
#define PATROL_INTERVAL		  3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST	 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME			  5.0f	// �ҋ@��Ԃ̎���

// �G�l�~�[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",														true,	122.0f	},	// �ҋ@
	{ "Character\\Enemy\\mutant\\anim\\mutant_walk.x",			true,	44.0f	},	// ���s
	{ "Character\\Enemy\\mutant\\anim\\mutant_run.x",			true,	27.0f	},	// ���s
	{ "Character\\Enemy\\mutant\\anim\\mutant_jump.x",			false,	96.0f	},	// �W�����v
	{ "Character\\Enemy\\mutant\\anim\\mutant_jump_attack.x",	false,	112.0f	},	// �W�����v�U��
	{ "Character\\Enemy\\mutant\\anim\\mutant_attack.x",		false,	81.0f	},	// �U��

};

// �R���X�g���N�^
CEnemy::CEnemy(std::vector<CVector> patrolPoints)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mAttackStartPos(CVector::zero)
	, mAttackEndPos(CVector::zero)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
{
	//���f���f�[�^�̎擾
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
	
	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// �v���C���[�����������ʒu�̃m�[�h���쐬
	mpLostPlayerNode = new CNavNode(CVector::zero, true);

	// ����|�C���g�Ɍo�H�T���p�̃m�[�h��z�u
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}
}

// �f�X�g���N�^
CEnemy::~CEnemy()
{
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

void CEnemy::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���ł����
	// ���g���폜���ꂽ
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// �X�V����
void CEnemy::Update()
{
	switch (mState)
	{
	case EState::eIdle:		UpdateIdle();		break;
	case EState::ePatrol:	UpdatePatrol();		break;
	case EState::eChase:	UpdateChase();		break;
	case EState::eLost:		UpdateLost();		break;
	case EState::eAttack:	UpdateAttack();		break;
	}

	CXCharacter::Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("��� : %s\n", GetStateStr(mState).c_str());
}

// �`�揈��
void CEnemy::Render()
{
	CXCharacter::Render();

	// ���񒆂ł���΁A
	if (mState == EState::ePatrol)
	{
		float rad = 1.0f;
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CMatrix m;
			m.Translate(mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f));
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawWireSphere(m, rad, c);
		}
	}
	// ����������Ԃł���΁A
	if (mState == EState::eLost)
	{
		//�v���C���[�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}

	CPlayer2* player = CPlayer2::Instance();
	CField2* field = CField2::Instance();
	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		//�v���C���[�Ƃ̊ԂɎՕ��������݂���ꍇ
		CHitInfo hit;
		if (field->CollisionRay(selfPos, playerPos, &hit))
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
			// �v���C���[�̈ʒu�܂ŗΐ���`��
			Primitive::DrawLine
			(
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	//// �c�����̏Փˏ���
	//if (self == mpColliderLine)
	//{
	//	if (other->Layer() == ELayer::eField)
	//	{
	//		// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
	//		CVector adjust = hit.adjust;
	//		adjust.X(0.0f);
	//		adjust.Z(0.0f);

	//		Position(Position() + adjust * hit.weight);
	//	}
	//}
	//// �������iX����Y���j�̏Փˏ���
	//else if (self == mpColliderLineX || self == mpColliderLineZ)
	//{
	//	if (other->Layer() == ELayer::eField)
	//	{
	//		// �����߂��x�N�g����Y�̒l��0�ɂ���
	//		CVector adjust = hit.adjust;
	//		adjust.Y(0.0f);
	//		// �����߂��x�N�g���̕��A���W���ړ�
	//		Position(Position() + adjust * hit.weight);
	//	}
	//}
}

// �A�j���[�V�����̐؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}

// ��Ԃ�؂�ւ�
void CEnemy::ChangeState(EState state)
{
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;

}

// �v���C���[������͈͓��ɓ��������ǂ���
bool CEnemy::IsFoundPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A����͈͊O�Ƃ���
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;

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
bool CEnemy::IsLookPlayer() const
{
	// �v���C���[�����݂��Ȃ��ꍇ�́A�����Ȃ�
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;
	// �t�B�[���h�����݂��Ȃ��ꍇ�́A�Օ������Ȃ��̂Ō�����
	CField2* field = CField2::Instance();
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

// �v���C���[���U���ł��邩�ǂ���
bool CEnemy::CanAttackPlayer() const
{
	// �v���C���[�����Ȃ��ꍇ�́A�U���ł��Ȃ�
	CPlayer2* player = CPlayer2::Instance();
	if ((player == nullptr)) return false;

	// �v���C���[�܂ł̋������U���͈͊O�ł���΁A�U���ł��Ȃ�
	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);	//�������l�����Ȃ�

	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// ���ׂĂ̏������N���A�����̂ŁA�U���ł���
	return true;
}

//�w�肵���ʒu�܂ňړ�����
bool CEnemy::MoveTo(const CVector& targetPos, float speed)
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
void CEnemy::ChangePatrolPoint()
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

// �ҋ@��Ԏ��̍X�V����
void CEnemy::UpdateIdle()
{
	// �v���C���[������͈͓��ɓ�������A�ǐՂɂ���
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// �ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// �ҋ@���Ԃ��o�߂�����A�����Ԃֈڍs
		ChangeState(EState::ePatrol);

		// 㩂�ݒu
		mpTrap = new CTrap(Position());

	}
}

// ���񒆂̍X�V����
void CEnemy::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	ChangeAnimation(EAnimType::eWalk);

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
		ChangeAnimation(EAnimType::eWalk);
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
		ChangeAnimation(EAnimType::eIdle);
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

// �ǐՎ��̍X�V����
void CEnemy::UpdateChase()
{
	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer2* player = CPlayer2::Instance();
	CVector targetPos = player->Position();

	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃɂ���
	if (!IsLookPlayer())
	{
		// ���������ʒu�ɂ́[����z�u
		mpLostPlayerNode->SetPos(targetPos);
		ChangeState(EState::eLost);
		return;
	}

	// �v���C���[�ɍU���ł���Ȃ�΁A�U����Ԃֈڍs
	if (CanAttackPlayer())
	{
		ChangeState(EState::eAttack);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eRun);

	// �o�H�T���Ǘ��N���X�����݂���΁A
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// ���g�̃m�[�h����v���C���[�̃m�[�h�܂ł̍ŒZ�o�H�����߂�
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// ���g�̃m�[�h����v���C���[�̃m�[�h�܂Ōq�����Ă�����A
			// �ړ�����ʒu�����̃m�[�h�̈ʒu�ɐݒ�
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// �ړ�����
	if (MoveTo(targetPos, RUN_SPEED))
	{
	}
}

// �v���C���[�������������̍X�V����
void CEnemy::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState(EState::eIdle);
		return;
	}
	// �v���C���[����������A�ǐՏ�Ԃֈڍs
	if (IsLookPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// ����A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eRun);

	switch (mStateStep)
	{
		// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
		case 0:
			if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
			{
				// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
				mNextMoveIndex = 1;
				mStateStep++;
			}
			else
			{
				// �o�H���Ȃ����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
				ChangeState(EState::eIdle);
			}
			break;
		case 1:
			// �v���C���[�����������ʒu�܂ňړ�
			if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
			{
				mNextMoveIndex++;
				if (mNextMoveIndex >= mMoveRoute.size())
				{
					// �ړ����I���Αҋ@��Ԃֈڍs
					ChangeState(EState::eIdle);
				}
			}
			break;
	}	
}

// �U�����̍X�V����
void CEnemy::UpdateAttack()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0 : �U���A�j���[�V�������Đ�
		case 0:
			// �U���J�n�ʒu�ƍU���I���ʒu��ݒ�
			mAttackStartPos = Position();
			mAttackEndPos = mAttackStartPos + VectorZ() * ATTACK_MOVE_DIST;

			ChangeAnimation(EAnimType::eJumpAttack);
			mStateStep++;
			break;
		// �X�e�b�v1 : �U�����̈ړ�����
		case 1:
		{
			// �U���A�j���[�V�������ړ��J�n�t���[���𒴂���
			float frame = GetAnimationFrame();
			if (frame >= ATTACK_MOVE_STAT)
			{
				//�ړ��I���t���[���܂œ��B���Ă��Ȃ�
				if (frame < ATTACK_MOVE_END)
				{
					// ���`��Ԃňړ��J�n�ʒu����ړ��I���ʒu�܂ňړ�����
					float moveFrame = ATTACK_MOVE_END - ATTACK_MOVE_STAT;
					float percent = (frame - ATTACK_MOVE_STAT) / moveFrame;
					CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
					Position(pos);
				}
				// �ړ��I���t���[���܂œ��B����
				else
				{
					Position(mAttackEndPos);
					mStateStep++;
				}
			}
			break;
		}
		// �X�e�b�v2 : �U���A�j���[�V�����̏I���҂�
		case 2:
			if (IsAnimationFinished())
			{
				mStateStep++;
			}
			break;
		// �X�e�b�v3 : �U���I����̑҂�����
		case 3:
			if (mElapsedTime < ATTACK_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// ���Ԃ��o�߂�����A�ҋ@��Ԃֈڍs
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// ��Ԃ̕�������擾
std::string CEnemy::GetStateStr(EState state) const
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
CColor CEnemy::GetStateColor(EState state) const
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
