#include "CSpider.h"
#include "CColliderSphere.h"
#include "CPlayer2.h"
#include "CCat.h"
#include "CNavNode.h"
#include "CNavManager.h"

#include "CDebugFieldOfView.h"

// �A�j���[�V�����̃p�X
#define ANIM_PATH "Character\\Enemy\\Warrok\\anim\\"

#define FOV_ANGLE			 45.0f	// ����͈͂̊p�x
#define FOV_LENGTH			100.0f	// ����͈͂̋���
#define EYE_HEIGHT			 10.0f	// ���_�̍���

#define ATTACK_COL_RADIUS 10.0f	// �U���R���C�_�[�̔��a
#define ATTACK_COL_POS CVector(0.0f, 5.0f, 8.0f)	// �U���R���C�_�[�̈ʒu

#define ATTACK_POWER 20		// �U����

#define SEARCH_RADIUS	 10.0f		// �󂹂�I�u�W�F�N�g��T�m����͈͂̔��a

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",							true,	60.0f,	1.0f	},	// �ҋ@		
	//{ ANIM_PATH"spider_walk.x",		true,	44.0f,	1.0f	},	// ���s
	//{ ANIM_PATH"spider_run.x",		true,	27.0f,	1.0f	},	// ���s
	//{ ANIM_PATH"spider_attack.x",	false,	81.0f,	1.0f	},	// �U��

};

// �R���X�g���N�^
CSpider::CSpider(std::vector<CVector> patrolPoints)
	: mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mpAttack1Col(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1�̎��Ɉ�ԋ߂��|�C���g�Ɉړ�
	, mNextMoveIndex(0)
{
	CPlayer2* target = CPlayer2::Instance();
	mTargets.push_back(target);

	CCat* cat = CCat::Instance();
	mTargets.push_back(cat);

	// �G��������
	InitEnemy("Spider", &ANIM_DATA);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation((int)EAnimType::eIdle);

	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		3.0f
	);
	// �t�B�[���h�ƁA�^�[�Q�b�g�̍U���R���C�_�[�ƃq�b�g����悤�ɐݒ�
	mpBodyCol->SetCollisionTags
	(
		{
			ETag::eField,
			ETag::eGimmick,
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{ ELayer::eFloor,
			ELayer::eWall,
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractObj,
			ELayer::eDoor
		}
	);

	// �U���R���C�_�[���쐬
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// �^�[�Q�b�g�̖{�̃R���C�_�[�Ƃ̂݃q�b�g����悤�ɐݒ�
	mpAttack1Col->SetCollisionTags
	(
		{
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject,
		}
	);
	mpAttack1Col->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eDoor,
		}
	);
	// �U���R���C�_�[�̍��W��ݒ�
	mpAttack1Col->Position(ATTACK_COL_POS);
	// �U���R���C�_�[���ŏ��̓I�t�ɂ��Ă���
	mpAttack1Col->SetEnable(false);

	// �󂹂�I�u�W�F�N�g��T�m����R���C�_�[���쐬
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	// �󂹂�I�u�W�F�N�g�̂ݏՓ˂���悤�ɐݒ�
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eDoor });

#if _DEBUG
	// ����͈͂̃f�o�b�O�\���N���X���쐬
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
#endif

	// �o�H�T���p�̃m�[�h���쐬
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// �^�[�Q�b�g�����������ʒu�̃m�[�h���쐬
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);

	// ����|�C���g�Ɍo�H�T���p�̃m�[�h��z�u
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

}

// �f�X�g���N�^
CSpider::~CSpider()
{
}

// �U������
bool CSpider::IsAttacking() const
{
	return false;
}

// �U���J�n
void CSpider::AttackStart()
{
}

// �U���I��
void CSpider::AttackEnd()
{
}

// �_���[�W���󂯂�
void CSpider::TakeDamage(int damage, CObjectBase* causer)
{
}

// ���S
void CSpider::Death()
{
}

// �Փˏ���
void CSpider::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// �I�u�W�F�N�g�폜��`����֐�
void CSpider::DeleteObject(CObjectBase* obj)
{
}

// �X�V
void CSpider::Update()
{
}

// �`��
void CSpider::Render()
{
	CEnemy::Render();
}

// ��Ԑ؂�ւ�
void CSpider::ChangeState(int state)
{
}

// �퓬����̕��֌���
void CSpider::LookAtBattleTarget(bool immediate)
{
}

// �v���C���[������͈͓��ɓ��������ǂ���
bool CSpider::IsFoundTarget(CObjectBase* target) const
{
	return false;
}

// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
bool CSpider::IsLookTarget(CObjectBase* target) const
{
	return false;
}

// �v���C���[���U���ł��邩�ǂ���
bool CSpider::CanAttackPlayer() const
{
	return false;
}

// �w�肵���ʒu�܂ňړ�����
bool CSpider::MoveTo(const CVector& targetPos, float speed)
{
	return false;
}

// ���ɏ��񂷂�|�C���g��ύX����
void CSpider::ChangePatrolPoint()
{
}

// �ҋ@��Ԃ̍X�V����
void CSpider::UpdateIdle()
{
}

// ��Ԃ̕�������擾
std::string CSpider::GetStateStr(EState state) const
{
	return std::string();
}

// ��Ԃ̐F���擾
CColor CSpider::GetStateColor(EState state) const
{
	return CColor();
}

// ��ԋ߂��ɂ���󂹂�I�u�W�F�N�g���擾
CInteractObject* CSpider::GetNearBreakObj() const
{
	return nullptr;
}
