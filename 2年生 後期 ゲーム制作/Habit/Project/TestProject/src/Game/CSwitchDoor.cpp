#include "CSwitchDoor.h"
#include "CPushSwitch.h"
#include "CSceneManager.h"

// �R���X�g���N�^
CSwitchDoor::CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& size)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	
{
	// ���݂̃V�[�����擾
	mScene = CSceneManager::Instance()->GetCurrentScene();
	switch (mScene)
	{
	case EScene::eTestGame:
		{
			// ���̃��f���f�[�^�擾(���͕ǂ�ݒ肵�Ă���)
			mpModel = CResourceManager::Get<CModel>("Wall");

			// ���̃R���C�_�[���擾(���͕ǂ̃R���C�_�[��ݒ肵�Ă���)
			CModel* colModel = CResourceManager::Get<CModel>("WallCol");
			mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

			// �ԐF��ݒ�
			mColor = CColor::red;

			break;
		}
	case EScene::eGame1:
		{
			// ���̃��f���f�[�^�擾(���͕ǂ�ݒ肵�Ă���)
			mpModel = CResourceManager::Get<CModel>("Gimmick_Wall");

			// ���̃R���C�_�[���擾(���͕ǂ̃R���C�_�[��ݒ肵�Ă���)
			CModel* colModel = CResourceManager::Get<CModel>("Gimmick_Wall");
			mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

			break;
		}
	}
	
	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CSwitchDoor::~CSwitchDoor()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

// �ڑ�����X�C�b�`��ǉ�����
void CSwitchDoor::AddSwitch(CPushSwitch* sw)
{
	mpSwitches.push_back(sw);
}

// ����������̍��W�ƊJ�������̍��W��ݒ肷��
void CSwitchDoor::SetAnimPos(const CVector& openPos, const CVector closePos)
{
	mOpenPos = openPos;
	mClosePos = closePos;
	Position(mIsOpened ? mOpenPos : mClosePos);
}

// �X�C�b�`���S�ĉ�����Ă��邩�ǂ���
bool CSwitchDoor::IsSwitchOn() const
{
	for (CPushSwitch* sw : mpSwitches)
	{
		if (!sw->IsOnSwtch()) return false;
	}
	return true;
}

// �X�V����
void CSwitchDoor::Update()
{
	// �J�A�j���[�V�����Đ���
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
		bool isSwitchOn = IsSwitchOn();

		if (isSwitchOn && !mIsOpened)
		{
			mIsOpened = true;
			mIsPlaying = true;
		}
		else if (!isSwitchOn && mIsOpened)
		{
			mIsOpened = false;
			mIsPlaying = true;
		}
	}
}

// �`�揈��
void CSwitchDoor::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}

