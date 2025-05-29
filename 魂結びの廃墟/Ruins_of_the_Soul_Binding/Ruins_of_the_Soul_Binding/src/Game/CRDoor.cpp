#include "CRDoor.h"

#define HP 15	// �̗�

// �R���X�g���N�^
CRDoor::CRDoor(const CVector& pos, const CVector& angle, const CVector& openPos)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	// ���̃��f���f�[�^�̎擾
	mpR_Door = CResourceManager::Get<CModel>("RightDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpR_DoorCol = CResourceManager::Get<CModel>("RightDoorCol");

	// ���̃R���C�_�[����
	mpR_DoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpR_DoorCol, true);
	mpR_DoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy });
	mpR_DoorColliderMesh->SetCollisionLayers
	(
		{ 
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy,
			ELayer::eAttackCol,
		}
	);

	mHp = HP;

	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	mInteractStr = "�܂��Ă���";

}

// �f�X�g���N�^
CRDoor::~CRDoor()
{
	if (mpR_DoorColliderMesh != nullptr)
	{
		delete mpR_DoorColliderMesh;
		mpR_DoorColliderMesh = nullptr;
	}
}

// ���ׂ�
void CRDoor::Interact()
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
	SetDebugName("�E�̔�");
#endif
}

// �X�V����
void CRDoor::Update()
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

	// HP��0�ɂȂ�����
	if (mHp <= 0)
	{
		// ����
		Kill();
	}
}

// �`�揈��
void CRDoor::Render()
{
	mpR_Door->Render(Matrix());
}
