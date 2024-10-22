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
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
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
	switch (mState)
	{
	case EState::eIdle:		UpdateIdle();
	case EState::ePatrol:	UpdatePatrol();
	case EState::eChase:	UpdateChase();
	case EState::eLost:		UpdateLost();
	case EState::eAttack:	UpdateAttack();
	}

	CXCharacter::Update();

	CDebugPrint::Print("��� : %s\n", GetStateStr(mState));
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

//��Ԃ�؂�ւ�
void CEnemy::ChangeState(EState state)
{
	if (state == mState) return;

	mState = state;
	mStateStep = 0;

}

//�ҋ@��Ԏ��̍X�V����
void CEnemy::UpdateIdle()
{
}

//���񒆂̍X�V����
void CEnemy::UpdatePatrol()
{
}

//�ǐՎ��̍X�V����
void CEnemy::UpdateChase()
{
}

//�v���C���[�������������̍X�V����
void CEnemy::UpdateLost()
{
}

//�U�����̍X�V����
void CEnemy::UpdateAttack()
{
}

//��Ԃ̕�������擾
std::string CEnemy::GetStateStr(EState state) const
{
	switch (state)
	{

	}

	return std::string();
}
