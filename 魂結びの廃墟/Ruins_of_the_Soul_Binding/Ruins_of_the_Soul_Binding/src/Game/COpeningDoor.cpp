#include "COpeningDoor.h"
#include "CNavManager.h"

#define HP 60			// �̗�
#define DOOR_WIDTH 11.0f	// �h�A�̕�

#define UI_OFFSET_POS_Y	10.0f
#define UI_OFFSET_POS_Z	 -4.95f

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

	// UI�̍��W��ݒ�
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;

	// ���̃��f���f�[�^�̎擾
	mpDoor = CResourceManager::Get<CModel>("OpeningDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpDoorCol = CResourceManager::Get<CModel>("OpeningDoorCol");

	// ���̃R���C�_�[����
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpDoorColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
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

// ���ׂ����Ԃ��ǂ���
bool COpeningDoor::CanInteract() const
{
	// �h�A���J���Ă���r���Ȃ�A���ׂ��Ȃ�
	if (mIsPlaying) return false;

	return true;
}

// ���ׂ�
void COpeningDoor::Interact()
{
	// TODO:�����܂��Ă�����A�J���Ȃ�

	// �܂��Ă���
	if (!mIsOpened)
	{
		mIsOpened = true;
		mIsPlaying = true;
	}
	// �J���Ă���
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

// ���ׂ�Ƃ��ɎQ�Ƃ���I�u�W�F�N�g�̈ʒu
CVector COpeningDoor::GetInteractPos() const
{
	// �J���˂͉�]�������_�Ȃ̂ŁA
	// ��]������h�A�̕��̔��������Y�������ʒu�𒲂ׂ�ʒu�Ƃ���
	return Position() + VectorX() * DOOR_WIDTH * 0.5f;
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
		CVector strtAngle = mIsOpened ? mCloseAngle : mOpenAngle;
		CVector endAngle = mIsOpened ? mOpenAngle : mCloseAngle;

		// ���Ԍo�߂ɂ��J�A�j���[�V����
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector angle = CVector::Lerp(strtAngle, endAngle, per);
			Rotation(CQuaternion(angle));
			mElapsedTime += Times::DeltaTime();
		}
		// �A�j���[�V�������Ԃ��o�߂���
		else
		{
			Rotation(CQuaternion(endAngle));
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}

	// UI�̍��W��ݒ�
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}

// �`�揈��
void COpeningDoor::Render()
{
	mpDoor->Render(Matrix());
}
