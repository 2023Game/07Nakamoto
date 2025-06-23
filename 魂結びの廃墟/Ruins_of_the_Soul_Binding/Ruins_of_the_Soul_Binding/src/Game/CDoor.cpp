#include "CDoor.h"
#include "CNavManager.h"

//#define MOVE_POS 9.9f	// �ړ�����
//#define MOVE_TIME 5.0f	// �ړ�����
#define HP 60			// �̗�

// �R���X�g���N�^
CDoor::CDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName , std::string colName)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mpOwner(nullptr)
{
	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);
	Rotate(angle);

	// ���̃��f���f�[�^�̎擾
	mpDoor = CResourceManager::Get<CModel>(modelName);
	// ���̃R���C�_�[�f�[�^�̎擾
	mpDoorCol = CResourceManager::Get<CModel>(colName);

	// ���̃R���C�_�[����
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy});
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
CDoor::~CDoor()
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
void CDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
void CDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// �h�A���J���Ă��邩�ǂ���
bool CDoor::IsOpened() const
{
	return mIsOpened;
}

// ���ׂ�
void CDoor::Interact()
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
void CDoor::Death()
{
	Kill();
}

// �Փ˔���
bool CDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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
void CDoor::Update()
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
				CVector pos = CVector::Lerp(mClosePos, mOpenPos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPos);
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
				CVector pos = CVector::Lerp(mOpenPos, mClosePos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePos);
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
void CDoor::Render()
{
	mpDoor->Render(Matrix());
}
