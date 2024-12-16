#include "CPlayer2.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CColliderSphere.h"
#include "CInteractObject.h"
#include "Maths.h"

#include "CDebugFieldOfView.h"

// �v���C���[�̃C���X�^���X
CPlayer2* CPlayer2::spInstatnce = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	  0.0f	},	// T�|�[�Y
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// �ҋ@
	{ "Character\\Player2\\anim\\pico_walk_s.x",	true,	 60.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_running.x",	true,	 44.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_jump_start.x",false,	 25.0f	},	// �W�����v�J�n
	{ "Character\\Player2\\anim\\pico_jumping.x",	false,	  1.0f	},	// �W�����v��
	{ "Character\\Player2\\anim\\pico_jump_end.x",	false,	 26.0f	},	// �W�����I���v
	{ "Character\\Player2\\anim\\pico_fall.x",		false,	 77.0f	},	// �]�|
	{ "Character\\Player2\\anim\\pico_death.x",		false,	109.0f	},	// ���S

	{ "Character\\Player2\\anim\\pico_crawl.x",		true,	 55.0f	},	// ����
	{ "Character\\Player2\\anim\\pico_sneak.x",		true,	 51.0f	},	// ���Ⴊ�݈ړ�
	{ "Character\\Player2\\anim\\pico_crouch_and_pick_up.x",		true,	180.0f	},	// ���Ⴊ��ŏE��
	
};

#define PLAYER_HEIGHT_CCOL1	12.0f		// �J�v�Z���R���C�_�[�̏�̍���
#define PLAYER_HEIGHT_CCOL2	 2.0f		// �J�v�Z���R���C�_�[�̉��̍���
#define PLAYER_WIDTH_CCOL	 2.5f		// �J�v�Z���R���C�_�[�̕�

#define MOVE_SPEED		  0.375f * 2.0f	// �������x
#define RUN_SPEED		  1.0f			// ���鑬�x

#define JUNP_MOVE_DIST	 20.0f			// �W�����v���̈ړ�����
#define JUNP_MOVE_START  16.0f			// �W�����v���̈ړ��J�n�t���[��
#define JUNP_MOVE_END	 33.0f			// �W�����v���̈ړ��I���t���[��
#define JUMP_SPEED		  1.0f			// �W�����v�̍���
#define GRAVITY			  0.0625f		// �d��

#define SEARCH_RADIUS	 10.0f			// ���ׂ�I�u�W�F�N�g��T�m����͈͂̔��a
#define FOV_ANGLE		 60.0f			// ����͈͂̊p�x
#define FOV_LENGTH		 10.0f			// ����͈͂̋���

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
	, mpSearchCol(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mpDebugFov(nullptr)
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
	mpColliderCapsule->SetCollisionLayers
	(
		{ ELayer::eField,
		  ELayer::eWall,
		  ELayer::eEnemy,
		  ELayer::eInteractObj,
		  ELayer::eAttackCol}
	);

	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, FOV_LENGTH);

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�[���쐬
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	// ���ׂ�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });
}

// �f�X�g���N�^
CPlayer2::~CPlayer2()
{
	// ����͈͂̃f�o�b�O�\�������݂�����A�ꏏ�ɍ폜����
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}

	SAFE_DELETE(mpColliderCapsule);
	SAFE_DELETE(mpSearchCol);

	spInstatnce = nullptr;
}

// �I�u�W�F�N�g�폜��`����
void CPlayer2::DeleteObject(CObjectBase* obj)
{
	// �폜���ꂽ�̂�����\���p�̃N���X�ł���΁A
	// �|�C���^����ɂ���
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// �C���X�^���X�̃|�C���^�̎擾
CPlayer2* CPlayer2::Instance()
{
	return spInstatnce;
}

// �X�V����
void CPlayer2::Update()
{
	if (mHp <= 0)
	{
		mState = EState::eDeath;
	}

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
			UpdateJumpStart();
			break;
		// �W�����v��
		case EState::eJump:
			UpdateJump();
			break;
		// �W�����v�I��
		case EState::eJumpEnd:
			UpdateJumpEnd();
			break;
		// �]�|
		case EState::eFall:
			UpdateFall();
			break;
		// ���S
		case EState::eDeath:
			UpdateDeath();
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
	CDebugPrint::Print("HP:%d\n", mHp);
	CDebugPrint::Print("ST:%d\n", mSt);

	// ���ׂ�I�u�W�F�N�g�̃��X�g���N���A
	mNearInteractObjs.clear();
}

// �X�e�[�^�X�𐮐��ɂ��Ď擾����
int CPlayer2::GetState()
{
	return static_cast<int>(mState);
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
		else
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

				// [E]�L�[����������A�߂��̒��ׂ�I�u�W�F�N�g�𒲂ׂ�
				if (CInput::PushKey('E'))
				{
					obj->Interact();
				}
			}
		}
	}
}

// �W�����v�J�n
void CPlayer2::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	float frame = GetAnimationFrame();
	mMoveSpeed = CVector::zero;

	if (frame >= JUNP_MOVE_START)
	{
		mMoveSpeedY += JUMP_SPEED;
		mIsGrounded = false;
		mState = EState::eJump;
	}
}

// �W�����v��
void CPlayer2::UpdateJump()
{
	ChangeAnimation(EAnimType::eJumping);
	if (mIsGrounded)
	{
		mState = EState::eJumpEnd;
	}
}

// �W�����v�I��
void CPlayer2::UpdateJumpEnd()
{
	if (mIsGrounded)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		ChangeAnimation(EAnimType::eJumpEnd);
		
		if (IsAnimationFinished())
		{
			mState = EState::eIdle;
		}
	}
}

// �]�|����
void CPlayer2::UpdateFall()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	ChangeAnimation(EAnimType::eFall);

	mSt++;
	
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;	
	}
}

void CPlayer2::UpdateDeath()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// ���S�A�j���[�V������ݒ�
	ChangeAnimation(EAnimType::eDeath);

	// TODO:�^�C�g���ɖ߂�ȂǂɕύX����
	if (CInput::Key('R') && IsAnimationFinished())
	{
		mState = EState::eIdle;
		mHp = 100;
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
		// �ҋ@��Ԃł���΁A
		if (mState == EState::eIdle)
		{
			if (CInput::Key(VK_SHIFT))
			{
				// �X�^�~�i������΁A
				if (mSt > 0)
				{
					// ���s�A�j���[�V�����ɐ؂�ւ���
					ChangeAnimation(EAnimType::eRun);

					mMoveSpeed += move * RUN_SPEED;
					mSt--;
				}
				else
				{
					mState = EState::eFall;
				}
			}
			else
			{
				// ���s�A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eWalk);

				mMoveSpeed += move * MOVE_SPEED;
				if (mSt < 100)
				{
					mSt++;
				}
			}
		}
		else if (mState == EState::eJump)
		{
			mMoveSpeed += move * RUN_SPEED;
		}
	}
	// �ړ��L�[����͂��Ă��Ȃ�
	else
	{
		// �ҋ@��Ԃł���΁A�ҋ@�A�j���[�V�����ɐ؂�ւ�
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);

			if (mSt < 100)
			{
				mSt++;
			}
		}
	}

	// [K]�L�[���������玀�S����iHP��0�ȉ��ɂȂ�����ɕύX����j
	if (CInput::Key('K'))
	{
		mState = EState::eDeath;
	}
}

// �Փ˔���
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �v���C���[�̑̂̓����蔻��
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
		// �X�C�b�`�Ƃ̓����蔻�菈��
		// �G�Ƃ̓����蔻�菈��
		else if (other->Layer() == ELayer::eInteractObj || other->Layer() == ELayer::eEnemy)
		{
			// �����߂��x�N�g��
			CVector adjust = hit.adjust;

			// �����߂��x�N�g���̕��A���W���ړ�
			Position(Position() + adjust * hit.weight);
		}
	}
	// ���ׂ�I�u�W�F�N�g�̒T�m�R���C�_�[�Ƃ̓����蔻��
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// �Փ˂������ׂ�I�u�W�F�N�g�����X�g�ɒǉ�
			mNearInteractObjs.push_back(obj);
		}
	}
}

// �_���[�W����
void CPlayer2::TakeDamege(int damage)
{
	mHp -= damage;
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
	case CPlayer2::EState::eFall:		return "eFall";
	case CPlayer2::EState::eDeath:		return "eDeath";

	case CPlayer2::EState::eCrawl:		return "eCrawl";
	case CPlayer2::EState::eSneak:		return "eSneak";
	case CPlayer2::EState::eCrouch_up:	return "eSneak";
	}
}

CInteractObject* CPlayer2::GetNearInteractObj() const
{
	// ��ԋ߂��̒��ׂ�I�u�W�F�N�g�̃|�C���^�i�[�p
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g�܂ł̋���
	CVector pos = Position();	// �v���C���[�̍��W���擾
	// �T�m�͈͓��̒��ׂ�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CInteractObject* obj : mNearInteractObjs)
	{

		// ���ݒ��ׂ��Ȃ���Ԃł���΁A�X���[
		if (!obj->CanInteract()) continue;

		// �I�u�W�F�N�g�̍��W���擾
		CVector objPos = obj->Position();
		// �v���C���[����I�u�W�F�N�g�܂ł̃x�N�g�������߂�
		CVector vec = objPos - pos;
		vec.Y(0.0f);	// �I�u�W�F�N�g�Ƃ̍����̍����l�����Ȃ�

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
