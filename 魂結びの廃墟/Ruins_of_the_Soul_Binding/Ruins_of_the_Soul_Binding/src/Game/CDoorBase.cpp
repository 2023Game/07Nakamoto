#include "CDoorBase.h"
#include "CNavManager.h"

#define DEFAULT_TEXT_PATH_OPEN		"UI\\Interact\\open.png"
#define DEFAULT_TEXT_PATH_CLOSE		"UI\\Interact\\close.png"

#define HP 60			// �̗�

// �R���X�g���N�^
CDoorBase::CDoorBase(const CVector& pos, const CVector& angle,
	std::string modelName, std::string colName)
	: mIsOpened(false)
	, mIsPlaying(false)
	, mpOwner(nullptr)
	, mpRoom(nullptr)
{
	Position(pos);
	Rotation(angle);

	// ���̃��f���f�[�^�̎擾
	mpDoor = CResourceManager::Get<CModel>(modelName);
	// ���̃R���C�_�[�f�[�^�̎擾
	mpDoorCol = CResourceManager::Get<CModel>(colName);

	// ���̃R���C�_�[����
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpDoorColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractSearch,
			ELayer::eBreakableSearch,
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
CDoorBase::~CDoorBase()
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
void CDoorBase::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
void CDoorBase::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// �ǂ̕����̔����ݒ�
void CDoorBase::SetRoom(CRoom* room)
{
	mpRoom = room;
}

// �ǂ̕����̔����Ԃ�
CRoom* CDoorBase::GetRoom() const
{
	return mpRoom;
}

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CDoorBase::GetInteractTextPath() const
{
	// �܂��Ă��邩
	if (mIsOpened)
	{
		// �܂��Ă�����
		return DEFAULT_TEXT_PATH_CLOSE;
	}
	else
	{
		// �J���Ă�����
		return DEFAULT_TEXT_PATH_OPEN;
	}
}

// �h�A���J���Ă��邩�ǂ���
bool CDoorBase::IsOpened() const
{
	return mIsOpened;
}

// ���ׂ����Ԃ��ǂ���
bool CDoorBase::CanInteract() const
{
	if (!mInteract) return false;
	if (mIsPlaying) return false;

	return true;
}

// ���ׂ�
void CDoorBase::Interact()
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
void CDoorBase::Death()
{
	Kill();
}

// �Փ˔���
bool CDoorBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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

// �`�揈��
void CDoorBase::Render()
{
	mpDoor->Render(Matrix());
}
