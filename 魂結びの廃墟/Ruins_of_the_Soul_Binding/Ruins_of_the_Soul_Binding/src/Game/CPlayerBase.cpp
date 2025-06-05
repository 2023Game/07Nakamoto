#include "CPlayerBase.h"
#include "CInteractObject.h"
#include "Maths.h"
#include "CPlayerManager.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CWarrok.h"

#define GRAVITY 0.0625f	// �d�͉����x
#define FOV_ANGLE		 60.0f		// ����͈͂̊p�x

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
	, mFovAngle(FOV_ANGLE)
	, mpSearchCol(nullptr)
	, mIsOperate(false)
	, mpCamera(nullptr)
{
	CPlayerManager::Instance()->AddPlayer(this);

	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// �f�X�g���N�^
CPlayerBase::~CPlayerBase()
{
	// �R���C�_�[�폜
	SAFE_DELETE(mpBodyCol);

	// �v���C���[�Ǘ��N���X�����݂�����A�v���C���[���X�g���玩�g����菜��
	CPlayerManager* pm = CPlayerManager::Instance();
	if (pm != nullptr)
	{
		pm->RemovePlayer(this);
	}
}

// ���삷�邩�ǂ����ݒ�
void CPlayerBase::SetOperate(bool operate)
{
	mIsOperate = operate;
	// ���̃v���C���[�𑀍삷��̂ł���΁A
	if (mIsOperate)
	{
		// ���̃v���C���[�p�̃J�����ɐ؂�ւ���
		mpCamera->SetCurrent(true);

		//TODO:�t�F�[�h�A�E�g�̏���
	}
}

// ���쒆�̃v���C���[���ǂ���
bool CPlayerBase::IsOperate() const
{
	return mIsOperate;
}

// ���쒆�̃J�����̃|�C���^��ݒ�
void CPlayerBase::SetCamera(CCamera* camera)
{
	mpCamera = camera;
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
		else if (other->Layer() == ELayer::eWall 
			|| other->Layer() == ELayer::eInteractObj
			|| other->Layer() == ELayer::eDoor)
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
	// ���ׂ�I�u�W�F�N�g�̒T�m�R���C�_�[�Ƃ̓����蔻��
	else if (self == mpSearchCol)
	{
		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr)
		{
			// ���ׂ�I�u�W�F�N�g�̍폜�t���O�������Ă��Ȃ�������
			if (!obj->IsKill())
			{
				// �Փ˂������ׂ�I�u�W�F�N�g�����X�g�ɒǉ�
				mNearInteractObjs.push_back(obj);
			}
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

CInteractObject* CPlayerBase::GetNearInteractObj() const
{
	// ��ԋ߂��̒��ׂ�I�u�W�F�N�g�̃|�C���^�i�[�p
	CInteractObject* nearObj = nullptr;
	float nearDist = 0.0f;	// ���݈�ԋ߂��ɂ��钲�ׂ�I�u�W�F�N�g�܂ł̋���
	CVector pos = Position();	// �v���C���[�̍��W���擾
	// �T�m�͈͓��̒��ׂ�I�u�W�F�N�g�����Ԃɒ��ׂ�
	for (CInteractObject* obj : mNearInteractObjs)
	{
		if (!obj->IsDeath())
		{
			// ���ݒ��ׂ��Ȃ���Ԃł���΁A�X���[
			if (!obj->CanInteract()) continue;
		}

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

// �X�V
void CPlayerBase::Update()
{
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();

	// ���ׂ�I�u�W�F�N�g�̃��X�g���N���A
	mNearInteractObjs.clear();
}

// �`��
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

