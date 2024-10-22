#include "CPlayer2.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// T�|�[�Y
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// �ҋ@
	{ "Character\\Player2\\anim\\pico_walk.x",		true,	30.0f	},	// ���s

};

// �R���X�g���N�^
CPlayer2::CPlayer2()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
{
	//���f���f�[�^�̎擾
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
}

CPlayer2::~CPlayer2()
{
}

//�X�V����
void CPlayer2::Update()
{
	CXCharacter::Update();
}

//�`�揈��
void CPlayer2::Render()
{
	CXCharacter::Render();
}

//�A�j���[�V�����̐؂�ւ�
void CPlayer2::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
