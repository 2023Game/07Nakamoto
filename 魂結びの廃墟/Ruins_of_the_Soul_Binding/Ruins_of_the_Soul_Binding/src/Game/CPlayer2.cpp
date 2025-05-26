//�v���C���[�N���X�̃C���N���[�h
#include "CPlayer2.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

#include "CCat.h"
#include "CCamera.h"
#include "CGameCamera2.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Player2\\Rusk\\anim\\"
#define BODY_HEIGHT 16.0f	// �{�̂̃R���C�_�[�̍���
#define BODY_RADIUS 3.0f	// �{�̂̃R���C�_�[�̕�
#define MOVE_SPEED 0.75f	// �ړ����x
#define JUMP_SPEED 1.5f		// �W�����v���x
#define GRAVITY 0.0625f		// �d�͉����x

// �v���C���[�̃C���X�^���X
CPlayer2* CPlayer2::spInstance = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	251.0f,	1.0f	},	// �ҋ@
	{ANIM_PATH"walk.x",			true,	31.0f,	0.5f	},	// ���s

};

// �R���X�g���N�^
CPlayer2::CPlayer2()
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//�C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	InitPlayer("Player2", &ANIM_DATA);

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
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eInteractObject, ETag::eRideableObject, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eInteractObj, ELayer::eEnemy, ELayer::eAttackCol });

}

// �f�X�g���N�^
CPlayer2::~CPlayer2()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);
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
		//// ���N���b�N�Ŏa���U���ֈڍs
		//if (CInput::PushKey(VK_LBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack1);
		//}
		//// �E�N���b�N�ŃL�b�N�U���ֈȍ~
		//else if (CInput::PushKey(VK_RBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack2);
		//}
		//// SPACE�L�[�ŃW�����v�J�n�ֈڍs
		//else if (CInput::PushKey(VK_SPACE))
		//{
		//	ChangeState(EState::eJumpStart);
		//}
	}
}

// �a��U��
void CPlayer2::UpdateAttack1()
{
}

// �R��U��
void CPlayer2::UpdateAttack2()
{
}

// �W�����v�J�n
void CPlayer2::UpdateJumpStart()
{
	//ChangeAnimation(EAnimType::eJumpStart);
	ChangeState((int)EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// �W�����v��
void CPlayer2::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		//ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState((int)EState::eJumpEnd);
	}
}

// �W�����v�I��
void CPlayer2::UpdateJumpEnd()
{
	// �W�����v�A�j���[�V�������I�����A
	// �n�ʂɐڒn������A�ҋ@��Ԃ֖߂�
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState((int)EState::eIdle);
	}
}

// ������
void CPlayer2::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
		// ������A�j���[�V�������J�n
		//ChangeAnimation(EAnimType::eHit, true);
		mStateStep++;
		break;
	case 1:
		// ������A�j���[�V�������I��������
		if (IsAnimationFinished())
		{
			// �ҋ@��Ԃֈڍs
			ChangeState((int)EState::eIdle);
			//ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

// �I�u�W�F�N�g�폜��`����
void CPlayer2::DeleteObject(CObjectBase* obj)
{
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

	if (CInput::PushKey('C'))
	{
		CCat* cat = CCat::Instance();
		CVector atPos = cat->Position() + CVector(0.0f, 10.0f, 0.0f);
		CCamera* camera = CCamera::MainCamera();

		camera->LookAt(atPos + CVector(0.0f, 0.0f, 40.0f), atPos, CVector::up);
		camera->SetFollowTargetTf(cat);
	}

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
		mMoveSpeed += move * MOVE_SPEED;

		// �ҋ@��Ԃł���΁A���s�A�j���[�V�����ɐ؂�ւ�
		if (mState == (int)EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eWalk);
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == (int)EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eIdle);
		}
	}
}

// �X�V
void CPlayer2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case (int)EState::eIdle:			UpdateIdle();		break;
		// �a��U��
	case (int)EState::eAttack1:		UpdateAttack1();	break;
		// �R��U��
	case (int)EState::eAttack2:		UpdateAttack2();	break;
		// �W�����v�J�n
	case (int)EState::eJumpStart:	UpdateJumpStart();	break;
		// �W�����v��
	case (int)EState::eJump:			UpdateJump();		break;
		// �W�����v�I��
	case (int)EState::eJumpEnd:		UpdateJumpEnd();	break;
		// ������
	case (int)EState::eHit:			UpdateHit();		break;
	}

	// �ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == (int)EState::eIdle
		|| mState == (int)EState::eJumpStart
		|| mState == (int)EState::eJump
		|| mState == (int)EState::eJumpEnd)
	{
		UpdateMove();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// �ړ�
	Position(Position() + moveSpeed);

	// �v���C���[���ړ������֌�����
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

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
	CXCharacter::Update();

	CVector pos = Position();
	//CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	//CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	//CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	//CDebugPrint::Print("PlayerState:%d\n", mState);

	mIsGrounded = false;

	//CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// �U������
bool CPlayer2::IsAttacking() const
{
	// �a��U����
	if (mState == (int)EState::eAttack1) return true;
	// �R��U����
	if (mState == (int)EState::eAttack2) return true;

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
	// �{�̂̃R���C�_�[�̏Փ˔���
	if (self == mpBodyCol)
	{
		// �t�B�[���h�Ƃ̏Փ�
		if (other->Layer() == ELayer::eField)
		{
			// �⓹�Ŋ���Ȃ��悤�ɁA�����߂��x�N�g����X��Z�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// �Փ˂����n�ʂ������V�䂩����ςŔ���
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// ���ς̌��ʂ��v���X�ł���΁A���ƏՓ˂���
			if (dot >= 0.0f)
			{
				// �����Ȃǂŏ��ɏォ��Փ˂������i���ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// �ڒn����
				mIsGrounded = true;
				// �ڒn�����n�ʂ̖@�����L�����Ă���
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
			}
			// ���ς̌��ʂ��}�C�i�X�ł���΁A�V��ƏՓ˂���
			else
			{
				// �W�����v�ȂǂœV��ɉ�����Փ˂������i��ړ��j�̂�
				// �㉺�̈ړ����x��0�ɂ���
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// �ǂƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::eWall || other->Layer() == ELayer::eInteractObj)
		{
			// �������ɂ̂݉����߂����߁A
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
		// �G�ƏՓ˂����ꍇ
		else if (other->Layer() == ELayer::eEnemy)
		{
			// �������ɂ̂݉����߂����߁A
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}