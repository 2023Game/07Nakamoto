#include "CEnemy2.h"
#include "CInput.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CDebugFieldOfView.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "Primitive.h"
#include "CField.h"
#include "CNavNode.h"
#include "CNavManager.h"

#define FOV_ANGLE			 45.0f	// ����͈͂̊p�x
#define FOV_LENGTH			100.0f	// ����͈͂̋���
#define EYE_HEIGHT			 10.0f	// ���_�̍���
#define WALK_SPEED			 50.0f	// �����Ă���Ƃ��̑��x
#define RUN_SPEED			 80.0f	// �����Ă�����Ƃ��̑��x
#define ROTATE_SPEED		 6.0f	// ��]���x

#define ENEMY_HEIGHT_CCOL1	24.0f	// �J�v�Z���R���C�_�[�̏�̍���
#define ENEMY_HEIGHT_CCOL2	2.0f	// �J�v�Z���R���C�_�[�̉��̍���
#define ENEMY_WIDTH_CCOL	5.0f	// �J�v�Z���R���C�_�[�̕�

#define ATTACK_RANGE		 20.0f	// �U���͈�
#define ATTACK_WAIT_TIME	  1.0f	// �U���I�����̑҂�����

#define ATTACK_JUDGE_START	39.0f	// �U������J�n�t���[��
#define ATTACK_JUDGE_END	46.0f	// �U������I���t���[��
#define ATTACK_DAMAGE		50		// �U���̃_���[�W

#define PATROL_INTERVAL		  3.0f	// ���̏���|�C���g�Ɉړ��J�n����܂ł̎���
#define PATROL_NEAR_DIST	 10.0f	// ����J�n���ɑI������鏄��|�C���g�̍ŒZ����
#define IDLE_TIME			  5.0f	// �ҋ@��Ԃ̎���

#define GAUGE_OFFSET_Y 15.0f

// �G�l�~�[2�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy2::AnimData CEnemy2::ANIM_DATA[] =
{
	{ "",													true,	60.0f   },	// �ҋ@		
	{ "Character\\Enemy\\warrok\\anim\\warrok_walk.x",		true,	44.0f	},	// ���s
	{ "Character\\Enemy\\warrok\\anim\\warrok_run.x",		true,	27.0f	},	// ���s
	{ "Character\\Enemy\\warrok\\anim\\warrok_attack.x",	false,	81.0f	},	// �U��

};

// �R���X�g���N�^
CEnemy2::CEnemy2(std::vector<CVector> patrolPoints)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
	, mAttackHit(false)
{
	//���f���f�[�^�̎擾
	CModelX* model = CResourceManager::Get<CModelX>("Enemy2");

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

	// �J�v�Z���R���C�_�\����
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, ENEMY_HEIGHT_CCOL2, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT_CCOL1, 0.0f),
		ENEMY_WIDTH_CCOL
	);
	mpColliderCapsule->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpColliderCapsule->SetCollisionLayers
	(
		{	ELayer::eField,
			ELayer::eWall,
			ELayer::ePlayer,
			ELayer::eInteractObj}
	);
	
	// �U���p�̋��R���C�_�\���쐬
	mpAttackCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		10.0f, true
	);
	mpAttackCollider->SetCollisionTags({ ETag::ePlayer });
	mpAttackCollider->SetCollisionLayers({ ELayer::ePlayer });
	mpAttackCollider->Position(0.0f, 5.0f, 10.0f);
	// �U���p�R���C�_�[���ŏ��̓I�t�ɂ��Ă���
	mpAttackCollider->SetEnable(false);

	//�r�ɃR���C�_�[��t����Ƃ�
	//// �U���p�̋��R���C�_�\���쐬
	//mpAttackCollider = new CColliderSphere
	//(
	//	this, ELayer::eAttackCol,
	//	40.0f,true
	//);
	//mpAttackCollider->SetCollisionTags({ ETag::ePlayer });
	//mpAttackCollider->SetCollisionLayers({ ELayer::ePlayer });

	//// ����̃{�[���̍s����擾
	//CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	//const CMatrix& mtx = frame->CombinedMatrix();
	//// �U���p�̃R���C�_�[������̍s��ɐݒ�
	//mpAttackCollider->SetAttachMtx(&mtx);
	//// �U���p�R���C�_�[���ŏ��̓I�t�ɂ��Ă���
	//mpAttackCollider->SetEnable(false);

	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// �v���C���[�����������ʒu�̃m�[�h���쐬
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);

	// ����|�C���g�Ɍo�H�T���p�̃m�[�h��z�u
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}
}

// �f�X�g���N�^
CEnemy2::~CEnemy2()
{
	SAFE_DELETE(mpColliderCapsule);
	SAFE_DELETE(mpAttackCollider);

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

void CEnemy2::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�I�u�W�F�N�g������͈͂̃f�o�b�O�\���ł����
	// ���g���폜���ꂽ
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// �X�V����
void CEnemy2::Update()
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
	mpAttackCollider->Update();

	// ���݂̏�Ԃɍ��킹�Ď���͈͂̐F��ύX
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("��� : %s\n", GetStateStr(mState).c_str());
}

// �`�揈��
void CEnemy2::Render()
{
	CXCharacter::Render();

	// ���񒆂ł���΁A
	if (mState == EState::ePatrol)
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
	if (mState == EState::eLost)
	{
		//�v���C���[�����������ʒu�Ƀf�o�b�O�\��
		float rad = 2.0f;
		Primitive::DrawBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f),
			CVector::one * rad,
			CColor::blue
		);
	}

	CPlayer2* player = CPlayer2::Instance();
	CField* field = CField::Instance();
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
void CEnemy2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �x�[�X�̏Փˏ������Ăяo��
	//CEnemy::Collision(self, other, hit);

	// �U���R���C�_�[���q�b�g����
	if (self == mpAttackCollider)
	{
		// �q�b�g�����̂��L�����N�^�[���A
		// �܂��U�����q�b�g���Ă��Ȃ��L�����N�^�[�ł����
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// �_���[�W��^����
			chara->TakeDamage(ATTACK_DAMAGE, this);
			// �U���q�b�g�ς݃��X�g�ɓo�^
			AddAttackHitObj(chara);
		}
	}
}

// �A�j���[�V�����̐؂�ւ�
void CEnemy2::ChangeAnimation(EAnimType type, bool restart)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength, restart);
}

// ��Ԃ�؂�ւ�
void CEnemy2::ChangeState(EState state)
{	
	// ���ɓ�����Ԃł���΁A�������Ȃ�
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �v���C���[������͈͓��ɓ��������ǂ���
bool CEnemy2::IsFoundPlayer() const
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
bool CEnemy2::IsLookPlayer() const
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

// �v���C���[���U���ł��邩�ǂ���
bool CEnemy2::CanAttackPlayer() const
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

// �w�肵���ʒu�܂ňړ�����
bool CEnemy2::MoveTo(const CVector& targetPos, float speed)
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
void CEnemy2::ChangePatrolPoint()
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

// �ҋ@��Ԏ��̍X�V����
void CEnemy2::UpdateIdle()
{
	// �v���C���[������͈͊O�ɓ�������A�ǐՂɂ���
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
	}
}

// ���񒆂̍X�V����
void CEnemy2::UpdatePatrol()
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
void CEnemy2::UpdateChase()
{
	// �v���C���[�̍��W�֌����Ĉړ�����
	CPlayer2* player = CPlayer2::Instance();
	CVector targetPos = player->Position();

	// �v���C���[�������Ȃ��Ȃ�����A����������Ԃɂ���
	if (!IsLookPlayer())
	{
		// ���������ʒu�Ƀm�[�h��z�u
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
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
		// �o�H�T���p�̃m�[�h���W���X�V
		mpNavNode->SetPos(Position());

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
void CEnemy2::UpdateLost()
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
			// �o�H���Ȃ����Ă��Ȃ���΁A�ҋ@��Ԃ֖߂�
			ChangeState(EState::eIdle);
			mpLostPlayerNode->SetEnable(false);
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
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// �U�����̍X�V����
void CEnemy2::UpdateAttack()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v�O�F�U���A�j���[�V�������Đ�
		case 0:
			// �x�[�X�N���X�̍U���J�n�������Ăяo��
			CXCharacter::AttackStart();

			ChangeAnimation(EAnimType::eAttack, true);
			mStateStep++;
			break;
		// �X�e�b�v�P: �U���p�R���C�_�[��L���ɂ���
		case 1:
			if (GetAnimationFrame() >= ATTACK_JUDGE_START)
			{
				mpAttackCollider->SetEnable(true);
				mStateStep++;
			}
			break;
		// �X�e�b�v�Q�F �U���p�R���C�_�[�𖳌��ɂ���
		case 2:
			if (GetAnimationFrame() >= ATTACK_JUDGE_END)
			{
				mpAttackCollider->SetEnable(false);
				mStateStep++;
			}
			break;
		// �X�e�b�v�R : �U���A�j���[�V�����̏I���҂�
		case 3:
			if (IsAnimationFinished())
			{
				mStateStep++;
				mAttackHit = false;
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
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// ��Ԃ̕�������擾
std::string CEnemy2::GetStateStr(EState state) const
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
CColor CEnemy2::GetStateColor(EState state) const
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