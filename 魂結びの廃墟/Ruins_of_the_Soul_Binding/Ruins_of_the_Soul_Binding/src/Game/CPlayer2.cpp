//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer2.h"
#include "CCat.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CCamera.h"
#include "CPlayerCamera.h"
#include "CInteractObject.h"
#include "CDebugFieldOfView.h"
#include "CGameUI.h"
#include "CHpGauge.h"
#include "CStGauge.h"
#include "CSceneManager.h"
#include "CDemonPower.h"
#include "CHandGlow.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Player2\\Rusk\\anim\\"

#define BODY_HEIGHT 16.0f	// �{�̂̃R���C�_�[�̍���
#define BODY_RADIUS 3.0f	// �{�̂̃R���C�_�[�̕�
#define MOVE_SPEED 0.75f	// �ړ����x
#define RUN_SPEED	1.0f	// ���鑬�x
#define JUMP_SPEED 1.5f		// �W�����v���x
#define GRAVITY 0.0625f		// �d�͉����x
#define ROTATE_SPEED	5.0f	// �ړ����̃v���C���[�̉�]���x

#define MAX_HP 100	// �̗͂̍ő�l
#define MAX_ST 100	// �X�^�~�i�̍ő�l
#define DEMON_POWER_DAMAGE	1	// �d�͂̌��ւ̃_���[�W

#define SEARCH_RADIUS	 10.0f		// ���ׂ�I�u�W�F�N�g��T�m����͈͂̔��a
#define FOV_ANGLE		 60.0f		// ����͈͂̊p�x
#define FOV_LENGTH		 10.0f		// ����͈͂̋���

#define CHANNELING_TIME	0.5f		// �d�͂𗬂�����Ń_���[�W������܂ł̎���

#define TRAIL_SPEED 15.0f		// �Ǐ]���̑��x
#define TRACKING_DIST 30.0f		// �v���C���[���痣�ꂽ����Ă��鋗��
#define MAX_DISTANCE 5.0f		// �Ǐ]���鋗�����X�V����ۂ̋���

// �v���C���[�̃C���X�^���X
CPlayer2* CPlayer2::spInstance = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	251.0f,	1.0f	},	// �ҋ@
	{ANIM_PATH"walk.x",			true,	31.0f,	0.5f	},	// ���s
	{ANIM_PATH"run.x",			true,	22.0f,	0.5f	},	// ���s
	{ANIM_PATH"hit.x",			false,	30.0f,	1.0f	},	// �_���[�W���󂯂���(��)
	{ANIM_PATH"died.x",			false,	174.0f,	1.0f	},	// ���s

	{ANIM_PATH"channeling_start.x",	false,	32.0f,	0.5f	},	// �d�͂𗬂����݊J�n
	{ANIM_PATH"channeling.x",		true,	9.0f,	0.3f	},	// �d�͂𗬂����ݒ�
	{ANIM_PATH"channeling_end.x",	false,	39.0f,	0.5f	},	// �d�͂𗬂����ݏI��

};

// �R���X�g���N�^
CPlayer2::CPlayer2()
	: mMaxSt(MAX_ST)
	, mSt(mMaxSt)
	, mChannelingTime(0)
#if _DEBUG
	,mpDebugFov(nullptr)
#endif
{
	// HP�̐ݒ�
	mMaxHp = MAX_HP;
	mHp = mMaxHp;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	InitPlayer("Player2", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

#if _DEBUG
	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, FOV_LENGTH);
#endif

	// �{�̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags
	(
		{
			ETag::eField,
			ETag::eInteractObject,
			ETag::eRideableObject,
			ETag::eEnemy,
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{
			ELayer::eField,
			ELayer::eWall,
			ELayer::eInteractObj,
			ELayer::eDoor,
			ELayer::eEnemy,
			ELayer::eDemon,
			ELayer::eAttackCol,
		}
	);

	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�[���쐬
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	// ���ׂ�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });

	// HP�Q�[�W�쐬
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	mpHpGauge->SetPos(0.0f, 0.0f);
	// �X�^�~�i�Q�[�W�̍쐬
	mpStGauge = new CStGauge();
	mpStGauge->SetMaxPoint(mMaxSt);
	mpStGauge->SetCurPoint(mSt);
	mpStGauge->SetPos(10.0f, 40.0f);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// �Ǐ]���ɏ�Q��������邽�߂̃m�[�h���쐬
	mpTrackingNode = new CNavNode(CVector::zero, true);
	mpTrackingNode->SetEnable(false);

}

// �f�X�g���N�^
CPlayer2::~CPlayer2()
{
#if _DEBUG
	// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜����
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
#endif

	// �R���C�_�[���폜
	SAFE_DELETE(mpSearchCol);

	spInstance = nullptr;

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpTrackingNode);
	}
}

// �C���X�^���X���擾
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}

// ��Ԃ�؂�ւ�
void CPlayer2::ChangeState(int state)
{
	// �U�����ɑ��ɏ�Ԃɕς�鎞�́A
	// �U���I���������Ăяo���Ă���
	if (IsAttacking())
	{
		AttackEnd();
	}

	CPlayerBase::ChangeState(state);
}

// �ҋ@
void CPlayer2::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		// �߂��̒��ׂ�I�u�W�F�N�g���擾
		CInteractObject* obj = GetNearInteractObj();
		if (obj != nullptr)
		{
#if _DEBUG
			// �T�m�͈͓��ɓ������I�u�W�F�N�g�̖��O��\��
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif

			// ���ׂ���I�u�W�F�N�g�̏�ɒ��ׂ�UI��\��
			//CGameUI::Instance()->ShowInteractUI(obj);

			// �߂��̒��ׂ���I�u�W�F�N�g���A�d�͂̌��ł���΁A
			if (CDemonPower* dp = dynamic_cast<CDemonPower*>(obj))
			{
				// ���N���b�N����������d�͂𗬂�����
				if (CInput::PushKey(VK_LBUTTON))
				{
					mpChannelingDemonPower = dp;
					ChangeState((int)EState::eChanneling);
				}
			}
			else
			{
				// [E]�L�[����������A�߂��̒��ׂ�I�u�W�F�N�g�𒲂ׂ�
				if (CInput::PushKey('E'))
				{
					if (obj->CanInteract())
					{
						obj->Interact();
					}
				}
			}
		}
		// �߂��ɒ��ׂ�I�u�W�F�N�g���Ȃ�����
		else
		{
			// ���ׂ�UI���\���ɂ���
			//CGameUI::Instance()->HideInteractUI();
		}
	}
}

// ������
void CPlayer2::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
		// ������A�j���[�V�������J�n
		ChangeAnimation((int)EAnimType::eHit, true);
		mStateStep++;
		break;
	case 1:
		// ������A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �ҋ@��Ԃֈڍs
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// ���S����
void CPlayer2::UpdateDeath()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// ���S�A�j���[�V������ݒ�
	ChangeAnimation((int)EAnimType::eDeath);

	// �A�j���[�V�������I��������A
	if (IsAnimationFinished())
	{
		// �Q�[���I�[�o�[�V�[����ǂݍ���
		//CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}
}

// �d�͂𒍂��ł���
void CPlayer2::UpdateChanneling()
{
	// TODO:�A�j���[�V�������J�n���鋗�����߂������痣�ꂽ�ʒu�ɍs���ď������J�n����

	switch (mStateStep)
	{
	case 0:
		// �d�͂𗬂����ރA�j���[�V�������J�n
		ChangeAnimation((int)EAnimType::eChannelingStart);
		mMoveSpeed.Set(0.0f, 0.0f, 0.0f);
		mChannelingTime = 0;
		mStateStep++;
		break;
	case 1:
		// �A�j���[�V�������I�������
		if (IsAnimationFinished())
		{
			// ���̃A�j���[�V������
			mStateStep++;
		}
		break;
	case 2:
		// ���N���b�N�������Ă����
		if (CInput::Key(VK_LBUTTON) && IsOperate())
		{
			// �d�͂𗬂����ރA�j���[�V����
			ChangeAnimation((int)EAnimType::eChanneling);
			mChannelingTime += Times::DeltaTime();

			if (mChannelingTime >= CHANNELING_TIME)
			{
				// �ڕW�Ƀ_���[�W��^����
				mpChannelingDemonPower->TakeDamage(DEMON_POWER_DAMAGE, this);
				// �ڕW���j�󂳂ꂽ��A���̃X�e�b�v��
				if (mpChannelingDemonPower->IsDeath())
				{
					mStateStep++;
				}
				mChannelingTime -= CHANNELING_TIME;
			}
		}
		else if (!IsOperate())
		{
			// �d�͂𗬂����ރA�j���[�V����
			ChangeAnimation((int)EAnimType::eChanneling);
			mChannelingTime += Times::DeltaTime();

			if (mChannelingTime >= CHANNELING_TIME)
			{
				// �ڕW�Ƀ_���[�W��^����
				mpChannelingDemonPower->TakeDamage(DEMON_POWER_DAMAGE, this);
				// �ڕW���j�󂳂ꂽ��A���̃X�e�b�v��
				if (mpChannelingDemonPower->IsDeath())
				{
					mStateStep++;
				}
				mChannelingTime -= CHANNELING_TIME;
			}
		}
		// �}�E�X�̍��{�^���𗣂�����A���̃X�e�b�v��
		else
		{
			mStateStep++;
		}
		break;
	case 3:
		// �d�͂𗬂����ޖڕW�̃|�C���^����ɂ���
		mpChannelingDemonPower = nullptr;
		// �d�̗͂������݂��I���A�j���[�V�������Đ��J�n
		ChangeAnimation((int)EAnimType::eChannelingEnd);
		mStateStep++;
		break;
	case 4:

		// �A�j���[�V�������I�������
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// �Ǐ]���
void CPlayer2::UpdateTracking()
{
	CCat* cat = CCat::Instance();
	CVector catPos = cat->Position();		// �v���C���[�܂ł̃x�N�g��
	CVector catVec = catPos - Position();	// �v���C���[�܂ł̋���

	// �x�N�g����2������߂�(�����s���y���̂�)
	float playerDist = catVec.LengthSqr();
	// �v���C���[�����苗�������ƁA���Ă������W���X�V
	if (playerDist >= TRACKING_DIST * TRACKING_DIST)	// �x�N�g����2��Ɣ�r����̂�TRACKING_DIST��2�悷��
	{
		const auto& trail = cat->GetTrail();
		size_t followIndex = 0;

		// �󂩂ǂ����𒲂ׂ�
		if (!trail.empty())
		{
			// �z��̔ԍ���2�Ԗڂ��擾����
			// 2�Ԗڂ܂Œ��g���͂��Ă��Ȃ��ꍇ�́A2�Ԗڂ�菬�����z����擾����
			followIndex = min(size_t(2), trail.size() - 1);
		}

		// ���Ă������W��ݒ�
		mFollowPos = trail[followIndex];
		// ���Ă������W�Ɍo�H�T���p�̃m�[�h��z�u
		mpTrackingNode->SetPos(mFollowPos);
		mpTrackingNode->SetEnable(true);

		// ���Ă������W�܂ł̌o�H��T������
		CNavManager::Instance()->Navigate(mpNavNode, mpTrackingNode, mTrackingRouteNodes);
		// �ړ��o�H����������
		if (mTrackingRouteNodes.size() >= 2)
		{
			mNextTrackingIndex = 1;
			mStateStep = 0;
		}
		else
		{
			ChangeAnimation((int)EAnimType::eIdle);

			mNextTrackingIndex = -1;
			mStateStep = -1;
		}
	}

	switch (mStateStep)
	{
		// �X�e�b�v0�F�ڕW�ʒu�܂ňړ�
	case 0:
		// ���Ɉړ�����m�[�h�ԍ����ݒ肳��Ă�����
		if (mNextTrackingIndex >= 0)
		{
			ChangeAnimation((int)EAnimType::eWalk);

			// ���Ɉړ�����m�[�h�܂ňړ����s��
			CNavNode* nextNode = mTrackingRouteNodes[mNextTrackingIndex];
			// �ړ��o�H�����������ꍇ
			if (MoveTo(nextNode->GetPos(), TRAIL_SPEED))
			{
				

				// �ړ����I���΁A���̃m�[�h��ړI�n�ɕύX
				mNextTrackingIndex++;
				// �ŏI�ړI�n�܂ňړ����I�����
				// (�ړ���̃m�[�h�ԍ����ړ��o�H�̃��X�g�̃T�C�Y�ȏゾ�����ꍇ)
				if (mNextTrackingIndex >= mTrackingRouteNodes.size())
				{
					mLookAtPos = catPos;
					mStateStep++;
				}
			}

			if (MoveTo(mFollowPos, TRAIL_SPEED))
			{
				mLookAtPos = catPos;
				mStateStep++;
			}

		}
		break;
		// �X�e�b�v1�F�v���C���[�̕���������
	case 1:
		{
			ChangeAnimation((int)EAnimType::eWalk);

			CVector targetDir = mLookAtPos - Position();
			targetDir.Y(0.0f);
			targetDir.Normalize();
			// ���X�Ɉړ������ֈړ�
			CVector forward = CVector::Slerp
			(
				VectorZ(),	// ���݂̐��ʕ���
				targetDir,	// �v���C���[�̕���
				ROTATE_SPEED * Times::DeltaTime()
			);
			Rotation(CQuaternion::LookRotation(forward));
			break;
		}
	}

	// ����t���O��true�ɂȂ�����A
	if (IsOperate())
	{
		// �ҋ@��Ԃɂ���
		ChangeState((int)EState::eIdle);
	}

}

// �I�u�W�F�N�g�폜��`����
void CPlayer2::DeleteObject(CObjectBase* obj)
{
#if _DEBUG
	// �폜���ꂽ�̂�����\���p�̃N���X�ł���΁A
	// �|�C���^����ɂ���
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
#endif
}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayer2::CalcMoveVec()
{
	CVector move = CVector::zero;

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���̓x�N�g���̒����œ��͂���Ă��邩����
	if (input.LengthSqr() > 0.0f)
	{
		// ������x�N�g��(�ݒu���Ă���ꍇ�́A�n�ʂ̖@��)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// �J�����̐��ʕ����x�N�g���Ə�����x�N�g���̊O�ς���
		// �������̈ړ��x�N�g�������߂�
		CVector moveSide = CVector::Cross(up, camForward);
		// �������̈ړ��x�N�g���Ə�����x�N�g���̊O�ς���
		// ���ʕ����̈ړ��x�N�g�������߂�
		CVector moveForward = CVector::Cross(moveSide, up);

		// ���߂��e�����̈ړ��x�N�g������A
		// �ŏI�I�ȃv���C���[�̈ړ��x�N�g�������߂�
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// �ړ��̍X�V����
void CPlayer2::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		// �ҋ@��Ԃł���΁A
		if (mState == (int)EState::eIdle)
		{
			if (CInput::Key(VK_SHIFT))
			{
				// �X�^�~�i������΁A
				if (mSt > 0)
				{
					// ���s�A�j���[�V�����ɐ؂�ւ���
					ChangeAnimation((int)EAnimType::eRun);

					mMoveSpeed += move * RUN_SPEED;
					mSt--;
				}
				else
				{
					// �]�|
					//ChangeState((int)EState::eFall);
				}
			}
			else
			{
				// ���s�A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation((int)EAnimType::eWalk);

				mMoveSpeed += move * MOVE_SPEED;
				if (mSt < mMaxSt)
				{
					mSt++;
				}
			}
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == (int)EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eIdle);

			if (mSt < mMaxSt)
			{
				mSt++;
			}
		}
	}
}

// �w�肵���ʒu�܂ňړ�����
bool CPlayer2::MoveTo(const CVector& targetPos, float speed)
{
	// �ړI�n�܂ł̃x�N�g�������߂�
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// �ړ������x�N�g�������߂�
	CVector moveDir = vec.Normalized();

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
	else if (remainDist <= 20.0f)
	{
		return true;
	}

	// ���X�Ɉړ������ֈړ�
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// ���݂̐��ʕ���
		moveDir,	// �ړ�����
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// �c��̋������ړ�������蒷���ꍇ�́A
	// �ړ��������ړI�n�ֈړ�����
	pos += forward * moveDist;
	Position(pos);

	// �ړI�n�ɂ͓������Ȃ�����
	return false;

}

// �X�V
void CPlayer2::Update()
{
	if (mHp <= 0)
	{
		ChangeState((int)EState::eDeath);
	}

	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
	// �ҋ@���
	case (int)EState::eIdle:		UpdateIdle();		break;
	// ������
	case (int)EState::eHit:			UpdateHit();		break;
	// ���S����
	case (int)EState::eDeath:		UpdateDeath();		break;
	// �d�͂𒍂��ł���
	case (int)EState::eChanneling:	UpdateChanneling();	break;
	// �Ǐ]���
	case(int)EState::eTracking:		UpdateTracking();	break;
	}

	// ���̃v���C���[�����쒆�ł���΁A
	if (IsOperate())
	{
		// �ҋ@���́A�ړ��������s��
		if (mState == (int)EState::eIdle)
		{
			UpdateMove();
		}


		// �d��
		mMoveSpeedY -= GRAVITY;

		// �ړ�
		CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
		Position(Position() + moveSpeed);

		// �v���C���[���ړ������֌�����
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));

		CVector p = Position();
		float distance = CVector::Distance(mLastPos, p);

		if (distance >= MAX_DISTANCE)
		{
			SetTrail();
		};
	}
	else
	{
		ChangeState((int)EState::eTracking);
	}

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �L�����N�^�[�̍X�V
	CPlayerBase::Update();

#if _DEBUG
	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);
	CDebugPrint::Print("FPS:%f\n", Times::FPS());
#endif

	// �n�ʂɂ��Ă��邩
	mIsGrounded = false;

	// �̗̓Q�[�W�̍X�V
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	// �X�^�~�i�Q�[�W�̍X�V����
	mpStGauge->SetMaxPoint(mMaxSt);
	mpStGauge->SetCurPoint(mSt);

#if _DEBUG
	// ���̏�ŕ���
	if (mState == (int)EState::eDeath)
	{
		if (CInput::PushKey('R'))
		{
			mHp = 100;
			mState = (int)EState::eIdle;
		}
	}
#endif // _DEBUG

}

// �X�e�[�^�X�𐮐��ɂ��Ď擾����
int CPlayer2::GetState()
{
	return static_cast<int>(mState);
}

// �U������
bool CPlayer2::IsAttacking() const
{
	// �U�����łȂ�
	return false;
}

// �U���J�n
void CPlayer2::AttackStart()
{
	// �x�[�X�N���X�̍U���J�n�������Ăяo��
	CXCharacter::AttackStart();
}

// �U���I��
void CPlayer2::AttackEnd()
{
	// �x�[�X�N���X�̍U���I���������Ăяo��
	CXCharacter::AttackEnd();
}

// �_���[�W���󂯂�
void CPlayer2::TakeDamage(int damage, CObjectBase* causer)
{
	// �x�[�X�N���X�̃_���[�W�������Ăяo��
	CXCharacter::TakeDamage(damage, causer);

	// ���S���Ă��Ȃ���΁A
	if (!IsDeath())
	{
		// �������Ԃֈڍs
		ChangeState((int)EState::eHit);

		// �U��������������̕����֌���
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// �ړ����~
		mMoveSpeed = CVector::zero;
	}
}

// �Փˏ���
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

// ���쒆���ǂ����ݒ�
void CPlayer2::SetOperate(bool operate)
{
	// �x�[�X�N���X�̏������Ăяo��
	CPlayerBase::SetOperate(operate);

	// ���g������v���C���[�̏ꍇ
	if (mIsOperate)
	{
		// ���g�𑀍삷�鎞��UI��\��
		mpHpGauge->SetShow(true);
		mpStGauge->SetShow(true);
	}
	// ���g������v���C���[�łȂ��ꍇ
	else
	{
		// ���g�����삷�鎞��UI���\��
		mpHpGauge->SetShow(false);
		mpStGauge->SetShow(false);
	}
}