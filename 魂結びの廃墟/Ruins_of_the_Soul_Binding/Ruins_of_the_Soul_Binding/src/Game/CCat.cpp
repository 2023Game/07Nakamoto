#include "CCat.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CPlayer2.h"
#include "CField.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Cat\\anim\\"

#define BODY_HEIGHT 12.0f	// �{�̂̃R���C�_�[�̍���
#define BODY_RADIUS 4.0f	// �{�̂̃R���C�_�[�̕�
#define MOVE_SPEED 0.75f	// �ړ����x
#define JUMP_SPEED 1.5f		// �W�����v���x
#define GRAVITY 0.0625f		// �d�͉����x

#define EYE_HEIGHT	7.0f	// ���_�̍���

#define TRAIL_SPEED 20.0f	// �Ǐ]���̑��x
#define TRACKING_DIST 30.0f	// �v���C���[���痣�ꂽ����Ă��鋗��

// �J�����̉�]���x
#define ROTATE_SPEED 1.5f

// �L�̃C���X�^���X
CCat* CCat::spInstance = nullptr;

// �L�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
		{ "",						true,	0.0f,	1.0f	},	// T�|�[�Y

};

// �R���X�g���N�^
CCat::CCat()
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// �L��������
	InitPlayer("Cat", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	// �{�̂̃R���C�_�[���쐬
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->Rotate(90.0f, 0.0f, 0.0f);
	mpBodyCol->Position(0.0f, BODY_RADIUS, 8.0f);

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
			ELayer::eEnemy,
			ELayer::eAttackCol,
			ELayer::eDoor,
		}
	);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::white);

	// �Ǐ]���ɏ�Q��������邽�߂̃m�[�h���쐬
	mpTrackingNode = new CNavNode(CVector::zero, true);
	mpTrackingNode->SetEnable(false);
}

// �f�X�g���N�^
CCat::~CCat()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);

	// �o�H�T���p�̃m�[�h��j��
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpTrackingNode);

		// ����m�[�h�ɔz�u�����m�[�h���S�č폜
		auto itr = mTrackingPoints.begin();
		auto end = mTrackingPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mTrackingPoints.erase(itr);
			delete del;
		}
	}
}

// �C���X�^���X���擾
CCat* CCat::Instance()
{
	return spInstance;
}

// ��Ԃ�؂�ւ�
void CCat::ChangeState(int state)
{
	// �U�����ɑ��ɏ�Ԃɕς�鎞�́A
	// �U���I���������Ăяo���Ă���
	if (IsAttacking())
	{
		AttackEnd();
	}

	CPlayerBase::ChangeState(state);
}

// ���݈ʒu����t�^�[�Q�b�g�������Ă��邩�ǂ���
bool CCat::IsLookTarget(CObjectBase* target) const
{
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

// �ҋ@
void CCat::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{

	}
}

// �Ǐ]
void CCat::UpdateTracking()
{
	CPlayer2 *player = CPlayer2::Instance();
	CVector playerPos = player->Position();		// �v���C���[�܂ł̃x�N�g��
	CVector playerVec = playerPos - Position();	// �v���C���[�܂ł̋���

	// �x�N�g����2������߂�(�����s���y���̂�)
	float playerDist = playerVec.LengthSqr();
	// �v���C���[�����苗�������ƁA���Ă������W���X�V
	if (playerDist >= TRACKING_DIST * TRACKING_DIST)	// �x�N�g����2��Ɣ�r����̂�TRACKING_DIST��2�悷��
	{
		const auto& trail = player->GetTrail();
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
		
		mpTrackingNode = new CNavNode(mFollowPos, true);
		mStateStep = 0;
	}

	if (!IsLookTarget(player))
	{
		mpTrackingNode->SetPos(player->Position());
		mpTrackingNode->SetEnable(true);
		ChangeState((int)EState::eLost);
		return;
	}

	switch (mStateStep)
	{
		// �X�e�b�v0�F�ڕW�ʒu�܂ňړ�
		case 0:
			if (MoveTo(mFollowPos, TRAIL_SPEED))
			{
				mLookAtPos = playerPos;
				mStateStep++;
			}
			break;
		// �X�e�b�v1�F�v���C���[�̕���������
		case 1:
		{
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

// �����������
void CCat::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState((int)EState::eIdle);
		return;
	}

	CPlayer2* player = CPlayer2::Instance();
	// �^�[�Q�b�g����������A�ǐՏ�Ԃֈڍs
	if (IsLookTarget(player))
	{
		ChangeState((int)EState::eTracking);
		return;
	}

	switch (mStateStep)
	{
		// �X�e�b�v0�F���������ʒu�܂ł̍ŒZ�o�H�����߂�
	case 0:
		// �o�H�T���p�̃m�[�h���W���X�V
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpTrackingNode, mMoveRoute))
		{
			// ���������ʒu�܂Ōo�H���q�����Ă�����A���̃X�e�b�v��
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// �o�H���Ȃ����Ă��Ȃ���΁A�Ǐ]��Ԃ֖߂�
			ChangeState((int)EState::eIdle);
			mpTrackingNode->SetEnable(false);
		}
		break;
	case 1:
		// �^�[�Q�b�g�����������ʒu�܂ňړ�
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), TRAIL_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// �ړ����I���ΒǏ]��Ԃֈڍs
				ChangeState((int)EState::eIdle);
				mpTrackingNode->SetEnable(false);
			}
		}
		break;
	}

	// ����L�����ɂȂ�����
	if (IsOperate())
	{
		ChangeState((int)EState::eIdle);
	}
}

bool CCat::IsAttacking() const
{
	return false;
}

// �I�u�W�F�N�g�폜��`����
void CCat::DeleteObject(CObjectBase* obj)
{
}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CCat::CalcMoveVec() const
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
		CCamera* mainCamera = CCamera::CurrentCamera();
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
void CCat::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// �v���C���[�̈ړ��x�N�g�������߂�
	CVector move = CalcMoveVec();
	// ���߂��ړ��x�N�g���̒����œ��͂���Ă��邩����
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == (int)EState::eIdle)
		{
			//ChangeAnimation(EAnimType::eWalk);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == (int)EState::eIdle)
		{
			//ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// �w�肵���ʒu�܂ňړ�����
bool CCat::MoveTo(const CVector& targetPos, float speed)
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
void CCat::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
	// �ҋ@���
	case (int)EState::eIdle:		UpdateIdle();		break;
	// �Ǐ]���
	case(int)EState::eTracking:		UpdateTracking();	break;
	// �����������
	case (int)EState::eLost:		UpdateLost();	break;
	}

	// ���̃v���C���[�𑀍쒆�ł���΁A
	if (IsOperate())
	{
		// �ҋ@���ƃW�����v���́A�ړ��������s��
		if (mState == (int)EState::eIdle
			|| mState == (int)EState::eJumpStart
			|| mState == (int)EState::eJump
			|| mState == (int)EState::eJumpEnd)
		{
			UpdateMove();
		}
	}
	else
	{
		mState == (int)EState::eTracking;
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// �L�𑀍쒆��������A
	if (mIsOperate)
	{
		// �}�E�X�̍��E�ړ��ŁA�L�����E�ɉ�]
		CVector2 delta = CInput::GetDeltaMousePos();
		Rotate(0.0f, delta.X() * ROTATE_SPEED * Times::DeltaTime(), 0.0f);
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

	// �z�C�[���N���b�N�Œe�۔���
	if (CInput::PushKey(VK_MBUTTON))
	{

	}

	// �uP�v�L�[����������A�Q�[�����I��
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// �L�����N�^�[�̍X�V
	CPlayerBase::Update();

	//CVector pos = Position();
	//CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	//CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	//CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	//CDebugPrint::Print("PlayerState:%d\n", mState);

	// �n�ʂɂ��Ă��邩
	mIsGrounded = false;

	//CDebugPrint::Print("FPS:%f\n", Times::FPS());

}

// �`��
void CCat::Render()
{
	// �L�𑀍삵�Ă�����A���̂Ȃ��L�̑̂�`��
	if (mIsOperate)
	{

	}
	// �L�𑀍삵�ĂȂ���΁A�S�g��`��
	else
	{
		CPlayerBase::Render();
	}
}

// �U���J�n
void CCat::AttackStart()
{
}

// �U���I��
void CCat::AttackEnd()
{
}

// �_���[�W���󂯂�
void CCat::TakeDamage(int damage, CObjectBase* causer)
{
}

// �Փˏ���
void CCat::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CPlayerBase::Collision(self, other, hit);
}

void CCat::SetOperate(bool operate)
{
	// �x�[�X�N���X�̏������Ăяo��
	CPlayerBase::SetOperate(operate);

	// ���g������v���C���[�̏ꍇ
	if (mIsOperate)
	{
		// ���g�𑀍삷�鎞��UI��\��
		//mpHpGauge->SetShow(true);
		//mpStGauge->SetShow(true);
	}
	// ���g������v���C���[�łȂ��ꍇ
	else
	{
		// ���g�����삷�鎞��UI���\��
		//mpHpGauge->SetShow(false);
		//mpStGauge->SetShow(false);
	}

}