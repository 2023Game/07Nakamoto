#include "CSwitchDoor.h"
#include "CSwitch.h"
#include "CNavManager.h"

//�R���X�g���N�^
CSwitchDoor::CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mIsOpened(false)
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
	mpModel = CResourceManager::Get<CModel>(modelName);
	// ���̃R���C�_�[�f�[�^�̎擾
	CModel* modelCol = CResourceManager::Get<CModel>(colName);

	// ���̃R���C�_�[����
	mpModelColliderMesh = new CColliderMesh(this, ELayer::eDoor, modelCol, true);
	mpModelColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpModelColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eEnemy ,
		}
	);

	// �o�H�T���p�̎Օ����`�F�b�N�̃R���C�_�[�ɁA���̃R���C�_�[��o�^
	CNavManager::Instance()->AddCollider(mpModelColliderMesh);
}

// �f�X�g���N�^
CSwitchDoor::~CSwitchDoor()
{
	// �R���C�_�[�̍폜
	if (mpModelColliderMesh != nullptr)
	{
		// �o�H�T���Ǘ��N���X�����݂�����A�Օ������X�g����h�A�̃R���C�_�[������菜��
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpModelColliderMesh);
		}
		delete mpModelColliderMesh;
		mpModelColliderMesh = nullptr;
	}

	// �����傪���݂���ꍇ�́A������Ɏ��g���폜���ꂽ���Ƃ�`����
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// �ڑ�����X�C�b�`��ǉ�
void CSwitchDoor::AddSwitch(CSwitch* sw)
{
	mpSwitches.push_back(sw);
}

// �������ݒ肷��
void CSwitchDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ���̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
void CSwitchDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// �h�A���J���Ă��邩�ǂ���
bool CSwitchDoor::IsOpened() const
{
	return mIsOpened;
}

// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
bool CSwitchDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	// ���̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpModelColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;
}

// �X�V����
void CSwitchDoor::Update()
{
	if (mIsPlaying)
	{
		CVector startPos = mIsOpened ? mClosePos : mOpenPos;
		CVector endPos = mIsOpened ? mOpenPos : mClosePos;

		// ���Ԍo�߂ɂ��J�A�j���[�V����
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector pos = CVector::Lerp(startPos, endPos, per);
			Position(pos);
			mElapsedTime += Times::DeltaTime();
		}
		// �A�j���[�V�������Ԃ��o�߂���
		else
		{
			Position(endPos);
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}
	else
	{
		bool isSwitchOn = IsAllSwitchOn();

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

// �X�C�b�`���S�ĉ�����Ă��邩�ǂ���
bool CSwitchDoor::IsAllSwitchOn() const
{
	for (CSwitch* sw : mpSwitches)
	{
		if (!sw->IsSwitchOn()) return false;
	}
	return true;
}

// �`�揈��
void CSwitchDoor::Render()
{
	mpModel->Render(Matrix());
}


