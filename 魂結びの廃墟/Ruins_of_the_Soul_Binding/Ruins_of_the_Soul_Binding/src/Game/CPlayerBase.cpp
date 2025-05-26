#include "CPlayerBase.h"

#define GRAVITY 0.0625f	// �d�͉����x

// �R���X�g���N�^
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
{
}

// �f�X�g���N�^
CPlayerBase::~CPlayerBase()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpBodyCol);
}

// �Փˏ���
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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

// �v���C���[�̏�����
void CPlayerBase::InitPlayer(std::string path, const std::vector<AnimData>* pAnimData)
{
	// �A�j���[�V�����f�[�^�e�[�u����ݒ�
	mpAnimData = pAnimData;

	// ���f���f�[�^��ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacter�̏�����
	Init(model);
}

// ��Ԑ؂�ւ�
void CPlayerBase::ChangeState(int state)
{
	// ������Ԃ̏ꍇ�͐؂�ւ��Ȃ�
	if (state == mState) return;

	// ��Ԃ�ύX���āA��Ԋ֘A�̕ϐ��̏�����
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// �A�j���[�V�����؂�ւ�
void CPlayerBase::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// �X�V
void CPlayerBase::Update()
{
	// �d��
	mMoveSpeedY -= GRAVITY;

	// �ړ�
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	mIsGrounded = false;
}

// �`��
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

