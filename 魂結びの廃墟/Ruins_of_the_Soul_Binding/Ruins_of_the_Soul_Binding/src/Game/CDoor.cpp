#include "CDoor.h"
#include "CNavManager.h"

//#define MOVE_POS 9.9f	// �ړ�����
//#define MOVE_TIME 5.0f	// �ړ�����
#define HP 15			// �̗�

// �R���X�g���N�^
CDoor::CDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName , std::string colName)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
{
	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);

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
		delete mpDoorColliderMesh;
		mpDoorColliderMesh = nullptr;
	}
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
	
	mInteractStr = mIsOpened ? "�܂��Ă���" : "�J���Ă���";

}

// ��ꂽ���̏���
void CDoor::Death()
{
	Kill();
}

// �X�V����
void CDoor::Update()
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
}

// �`�揈��
void CDoor::Render()
{
	mpDoor->Render(Matrix());
}
