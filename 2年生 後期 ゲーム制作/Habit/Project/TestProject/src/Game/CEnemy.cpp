#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",										true,	0.0f	},	// T�|�[�Y
	//{ "Character\\Player2\\anim\\idle.x",			true,	122.0f	},	// �ҋ@
	//{ "Character\\Model\\vampire.x",		false,	91.0f	},	// �U��

};

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
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
	ChangeAnimation(EAnimType::eTPose);
}

CEnemy::~CEnemy()
{
}

//�X�V����
void CEnemy::Update()
{
	CXCharacter::Update();
}

//�`�揈��
void CEnemy::Render()
{
	CXCharacter::Render();
}

//�A�j���[�V�����̐؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
