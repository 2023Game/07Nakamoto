#include "CPlayer3.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CColliderSphere.h"
#include "CInteractObject.h"
#include "Maths.h"
#include "CGaugeUI.h"
#include "CHpGauge.h"
#include "CStGauge.h"
#include "CSceneManager.h"

#include "CDebugFieldOfView.h"

// �v���C���[�̃C���X�^���X
CPlayer3* CPlayer3::spInstatnce = nullptr;

// �v���C���[�̃A�j���[�V�����f�[�^�̃e�[�u��
const CPlayer3::AnimData CPlayer3::ANIM_DATA[] =
{
	{ "",											true,	  0.0f	},	// T�|�[�Y
};

// �R���X�g���N�^
CPlayer3::CPlayer3()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eTPose)
{
	// �C���X�^���X�̐ݒ�
	spInstatnce = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Ghost");

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

// �f�X�g���N�^
CPlayer3::~CPlayer3()
{
	spInstatnce = nullptr;
}

// �C���X�^���X�̃|�C���^�̎擾
CPlayer3* CPlayer3::Instance()
{
	return spInstatnce;
}

// �X�V����
void CPlayer3::Update()
{
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();
}

// �X�e�[�^�X�𐮐��ɂ��Ď擾����
int CPlayer3::GetState()
{
	return static_cast<int>(mState);
}

// �Փ˔���
void CPlayer3::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �_���[�W����
void CPlayer3::TakeDamege(int damage)
{
}

// �`�揈��
void CPlayer3::Render()
{
	CXCharacter::Render();
}

// �A�j���[�V�����̐؂�ւ�
void CPlayer3::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}
