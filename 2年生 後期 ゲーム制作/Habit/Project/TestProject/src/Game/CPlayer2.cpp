#include "CPlayer2.h"
#include "CInput.h"

// �v���C���[�̃C���X�^���X
CPlayer2* CPlayer2::spInstatnce = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// �ҋ@
	{ "Character\\Player2\\anim\\pico_walk.x",		true,	30.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_run.x",		true,	22.0f	},	// ���s
	{ "Character\\Player2\\anim\\pico_jump.x",		true,	51.0f	},	// �W�����v
	{ "Character\\Player2\\anim\\pico_crawl.x",		true,	55.0f	},	// ����
	{ "Character\\Player2\\anim\\pico_sneak.x",		true,	51.0f	},	// ���Ⴊ�݈ړ�
	{ "Character\\Player2\\anim\\pico_crouch_and_pick_up.x",		true,	180.0f	},	// ���Ⴊ��ŏE��

	
};

#define PLAYER_HEIGHT	16.0f	
#define MOVE_SPEED		10.0f	// �������x
#define RUN_SPEED		20.0f	// ���鑬�x

// �R���X�g���N�^
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
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

	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

// �f�X�g���N�^
CPlayer2::~CPlayer2()
{
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}

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
	case EState::eIdle:
		break;
	
	}

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();
}

CVector CPlayer2::CalcMoveVec() const
{
	return CVector();
}

void CPlayer2::UpdateIdle()
{
}

void CPlayer2::UpdateJumpStart()
{
}

void CPlayer2::UpdateMove()
{
}

// �Փ˔���
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
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
