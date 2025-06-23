#include "COpeningDoor.h"
#include "CNavManager.h"

#define HP 60			// �̗�

// �R���X�g���N�^
COpeningDoor::COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mpOwner(nullptr)
{
	mOpenAngle = openAngle;
	mCloseAngle = angle;
	Rotate(mIsOpened ? mOpenAngle : mCloseAngle);
	Position(pos);

	// ���̃��f���f�[�^�̎擾
	mpDoor = CResourceManager::Get<CModel>("RightDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpDoorCol = CResourceManager::Get<CModel>("RightDoorCol");

	// ���̃R���C�_�[����
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy });
	mpDoorColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy ,
			ELayer::eAttackCol
		}
	);

	// �o�H�T���p�̎Օ����`�F�b�N�̃R���C�_�[�ɁA���̃R���C�_�[��o�^
	CNavManager::Instance()->AddCollider(mpDoorColliderMesh);

	mHp = HP;

	mInteractStr = "�܂��Ă���";
}

// �f�X�g���N�^
COpeningDoor::~COpeningDoor()
{
	// �R���C�_�[�̍폜
	if (mpDoorColliderMesh != nullptr)
	{
		// �o�H�T���Ǘ��N���X�����݂�����A�Օ������X�g����h�A�̃R���C�_�[������菜��
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpDoorColliderMesh);
		}
		SAFE_DELETE(mpDoorColliderMesh);
	}

	// �����傪���݂���ꍇ�́A������Ɏ��g���폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �������ݒ肷��
void COpeningDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
void COpeningDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// �h�A���J���Ă��邩�ǂ���
bool COpeningDoor::IsOpened() const
{
	return mIsOpened;
}

// ���ׂ�
void COpeningDoor::Interact()
{
	// �܂��Ă���
	if (!mIsOpened)
	{
		mIsOpened = true;
		mIsPlaying = true;
	}
	else if (mIsOpened)
	{
		mIsOpened = false;
		mIsPlaying = true;
	}

	// �؂�ւ����ɌĂяo���֐����ݒ肳��Ă�����A
	// ���̊֐����Ăяo��
	if (mOnChangeFunc != nullptr)
	{
		mOnChangeFunc();
	}

	mInteractStr = mIsOpened ? "�܂��Ă���" : "�J���Ă���";
}

// ��ꂽ���̏���
void COpeningDoor::Death()
{
	Kill();
}

// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
bool COpeningDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	// ���̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpDoorColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;

}

// �X�V����
void COpeningDoor::Update()
{
	if (mIsPlaying)
	{
		mInteract = false;
		// �����J���A�j���[�V����
		if (mIsOpened)
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector angle = CVector::Lerp(mCloseAngle, mOpenAngle, per);
				Rotate(angle);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Rotate(mOpenAngle);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
				mInteract = true;
			}
		}
		// �������A�j���[�V����
		else
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector angle = CVector::Lerp(mOpenAngle, mCloseAngle, per);
				Rotate(angle);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Rotate(mCloseAngle);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	// �J���ł͂Ȃ�
	else
	{
		mInteract = true;
	}

}

// �`�揈��
void COpeningDoor::Render()
{
	mpDoor->Render(Matrix());
}
