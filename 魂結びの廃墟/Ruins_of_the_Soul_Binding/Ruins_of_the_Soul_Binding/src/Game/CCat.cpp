#include "CCat.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CNavNode.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Cat\\anim\\"

#define BODY_HEIGHT 12.0f	// �{�̂̃R���C�_�[�̍���
#define BODY_RADIUS 4.0f	// �{�̂̃R���C�_�[�̕�
#define MOVE_SPEED 0.75f	// �ړ����x
#define JUMP_SPEED 1.5f		// �W�����v���x
#define GRAVITY 0.0625f		// �d�͉����x

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

}

// �f�X�g���N�^
CCat::~CCat()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpBodyCol);
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

// �ҋ@
void CCat::UpdateIdle()
{
	// �ڒn���Ă���΁A
	if (mIsGrounded)
	{

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

// �X�V
void CCat::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
	// �ҋ@���
	case (int)EState::eIdle:			UpdateIdle();		break;
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