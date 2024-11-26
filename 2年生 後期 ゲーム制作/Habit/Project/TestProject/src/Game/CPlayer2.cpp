#include "CPlayer2.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"

// �v���C���[�̃C���X�^���X
CPlayer2* CPlayer2::spInstatnce = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// �ҋ@
	{ "Character\\Player2\\anim\\pico_walk_s.x",	true,	60.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_running.x",	true,	44.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_jump_start.x",false,	25.0f	},	// �W�����v�J�n
	{ "Character\\Player2\\anim\\pico_jumping.x",	false,	 1.0f	},	// �W�����v��
	{ "Character\\Player2\\anim\\pico_jump_end.x",	false,	26.0f	},	// �W�����I���v

	{ "Character\\Player2\\anim\\pico_crawl.x",		true,	55.0f	},	// ����
	{ "Character\\Player2\\anim\\pico_sneak.x",		true,	51.0f	},	// ���Ⴊ�݈ړ�
	{ "Character\\Player2\\anim\\pico_crouch_and_pick_up.x",		true,	180.0f	},	// ���Ⴊ��ŏE��
	
};

#define PLAYER_HEIGHT_CCOL1	12.0f		// �v���C���[�̍���
#define PLAYER_HEIGHT_CCOL2	2.0f		// �v���C���[�̍���
#define PLAYER_WIDTH_CCOL	2.5f		// �v���C���[�̕�

#define MOVE_SPEED		0.375f * 2.0f	// �������x
#define RUN_SPEED		1.0f			// ���鑬�x

#define JUNP_MOVE_DIST	20.0f			// �W�����v���̈ړ�����
#define JUNP_MOVE_START	16.0f			// �W�����v���̈ړ��J�n�t���[��
#define JUNP_MOVE_END	33.0f			// �W�����v���̈ړ��I���t���[��
#define JUMP_SPEED		1.0f			// �W�����v�̍���
#define GRAVITY			0.0625f			// �d��

// �R���X�g���N�^
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mHp(100)
	, mSt(100)
{
	// �C���X�^���X�̐ݒ�
	spInstatnce = this;

	// ���f���f�[�^�̎擾
	CModelX* model = CResourceManager::Get<CModelX>("Player2");

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
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_HEIGHT_CCOL2, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT_CCOL1, 0.0f),
		PLAYER_WIDTH_CCOL
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall});

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// �f�X�g���N�^
CPlayer2::~CPlayer2()
{
	SAFE_DELETE(mpColliderCapsule);

	spInstatnce = nullptr;
}

// �C���X�^���X�̃|�C���^�̎擾
CPlayer2* CPlayer2::Instance()
{
	return spInstatnce;
}

// �X�V����
void CPlayer2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�ύX������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
		case EState::eIdle:
			UpdateIdle();
			break;
		// �W�����v�J�n
		case EState::eJumpStart:
			UpdateJump();
			break;
		// �W�����v��
		case EState::eJump:
			UpdateJump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			UpdateJump();
			break;
	}

	//�ҋ@���ƃW�����v���́A�ړ��������s��
	if (mState == EState::eIdle
		|| mState == EState::eJump)
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

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// �o�H�T���p�̃m�[�h�����݂���΁A���W���X�V
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("State:%s\n", ToString(mState).c_str());

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// �ҋ@����
void CPlayer2::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{
		// SPACE�L�[�ŃW�����v
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// �W�����v����
void CPlayer2::UpdateJump()
{
	// �X�e�b�v���Ƃɏ�����؂�ւ���
	switch (mStateStep)
	{
		// �X�e�b�v0�F�W�����v�J�n�A�j���[�V�������Đ�
		case 0:
			ChangeAnimation(EAnimType::eJumpStart);
			mStateStep++;
			break;
		// �X�e�b�v1�F�W�����v���̈ړ�����
		case 1:
		{
			float frame = GetAnimationFrame();
			mMoveSpeed = CVector::zero;

			if (frame >= JUNP_MOVE_START)
			{
				mMoveSpeedY += JUMP_SPEED;
				mIsGrounded = false;
				mState = EState::eJump;
				mStateStep++;
			}
			break;
		}
		// �X�e�b�v2�F�W�����v�J�n�A�j���[�V�����̏I���҂�
		case 2:
		{
			if (IsAnimationFinished())
			{
				ChangeAnimation(EAnimType::eJumping);
				mStateStep++;
			}
			break;
		}
		// �X�e�b�v2�F���n�҂�
		case 3:
			if (mIsGrounded)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				ChangeAnimation(EAnimType::eJumpEnd);
				mState = EState::eJumpEnd;
				mStateStep++;
			}
			break;
		// �X�e�b�v�S�F�W�����v�I���҂� 
		case 4:
			if (IsAnimationFinished())
			{
				mState = EState::eIdle;
				mStateStep = 0;
			}
			break;
	}
}

// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
CVector CPlayer2::CalcMoveVec() const
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
		if (CInput::Key(VK_SHIFT))
		{
			// �X�^�~�i������΁A
			if (mSt > 0)
			{
				mMoveSpeed += move * RUN_SPEED;
				mSt--;
			}
			// �X�^�~�i���Ȃ���Ԃő�������A
			else
			{

			}
		}
		else
		{
			mMoveSpeed += move * MOVE_SPEED;
			mSt++;
		}
		
		// �ҋ@��Ԃł���΁A
		if (mState == EState::eIdle)
		{
			if (CInput::Key(VK_SHIFT))
			{
				// ���s�A�j���[�V�����ɐ؂�ւ���
				ChangeAnimation(EAnimType::eRun);
			}
			else
			{
				// ���s�A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eWalk);
			}
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// �Փ˔���
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{
		// �t�B�[���h�ƓV��̓����蔻�菈��
		if (other->Layer() == ELayer::eField)
		{
			CVector adjust = hit.adjust;
			// �����߂��x�N�g���̕��A���W���ړ�
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
		// �ǂƂ̓����蔻�菈��
		else if (other->Layer() == ELayer::eWall)
		{
			// �����߂��x�N�g����Y�̒l��0�ɂ���
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
}

// �`�揈��
void CPlayer2::Render()
{
	CXCharacter::Render();
}

// �A�j���[�V�����̐؂�ւ�
void CPlayer2::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}

// ���̏�Ԃ���ʂɕ\��
std::string CPlayer2::ToString(EState state)
{
	switch (mState)
	{
	case CPlayer2::EState::eTPose:		return "eTPose";
	case CPlayer2::EState::eIdle:		return "eIdle";
	case CPlayer2::EState::eWalk:		return "eWalk";
	case CPlayer2::EState::eRun:		return "eRun";
	case CPlayer2::EState::eJumpStart:	return "eJumpStart";
	case CPlayer2::EState::eJump:		return "eJump";
	case CPlayer2::EState::eJumpEnd:	return "eJumpEnd";
	case CPlayer2::EState::eCrawl:		return "eCrawl";
	case CPlayer2::EState::eSneak:		return "eSneak";
	case CPlayer2::EState::eCrouch_up:	return "eSneak";
	}
}
