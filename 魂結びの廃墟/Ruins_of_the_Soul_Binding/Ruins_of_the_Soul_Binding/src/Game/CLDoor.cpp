#include "CLDoor.h"

//#define MOVE_POS 9.9f	// �ړ�����
//#define MOVE_TIME 5.0f	// �ړ�����
#define HP 15			// �̗�

// �R���X�g���N�^
CLDoor::CLDoor(const CVector& pos, const CVector& angle,const CVector& openPos)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mIsBroken(false)
{
	// ���̃��f���f�[�^�̎擾
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// ���̃R���C�_�[����
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpL_DoorCol, true);
	mpL_DoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy});
	mpL_DoorColliderMesh->SetCollisionLayers
	(
		{ 
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy ,
			ELayer::eAttackCol
		}
	);

	mHp = HP;

	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	mInteractStr = "�܂��Ă���";
}

// �f�X�g���N�^
CLDoor::~CLDoor()
{
	// �R���C�_�[�̍폜
	if (mpL_DoorColliderMesh != nullptr)
	{
		delete mpL_DoorColliderMesh;
		mpL_DoorColliderMesh = nullptr;
	}
}

// ���ׂ�
void CLDoor::Interact()
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
	
	mInteractStr = mIsOpened ? "�܂��Ă���" : "�J���Ă���";

#if _DEBUG
	SetDebugName("���̔�");
#endif
}

// �X�V����
void CLDoor::Update()
{
	if (mIsPlaying)
	{
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
	}		

	if (mIsBroken)
	{
		Kill();
	}

	// HP��0�ɂȂ�����
	if (mHp <= 0)
	{
		mIsBroken = true;	
	}
}

// �`�揈��
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
