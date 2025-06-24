#include "CSwitchDoor.h"
#include "CColliderMesh.h"
#include "CSwitch.h"
#include "CNavNode.h"
#include "CNavManager.h"

// �R���X�g���N�^
CSwitchDoor::CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
	, mpOwner(this)
{
	mClosePos = pos;
	mOpenPos = openPos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	Rotation(angle);

	mpModel = CResourceManager::Get<CModel>("");

	CModel* mesh = CResourceManager::Get<CModel>("");
	mpColliderMesh = new CColliderMesh(this, ELayer::eSwitch, mesh, true);
	mpColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eEnemy ,
		}
	);
}

// �f�X�g���N�^
CSwitchDoor::~CSwitchDoor()
{
	// �R���C�_�[�̍폜
	if (mpColliderMesh != nullptr)
	{
		// �o�H�T���Ǘ��N���X�����݂�����A�Օ������X�g����R���C�_�[������菜��
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpColliderMesh);
		}
		SAFE_DELETE(mpColliderMesh);
	}
}

// �ڑ�����X�C�b�`��ǉ�
void CSwitchDoor::AddSwitch(CSwitch* sw)
{
	mpSwitches.push_back(sw);
}

bool CSwitchDoor::IsOpened() const
{
	return mIsOpened;
}


bool CSwitchDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	// ���̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;
}

// �X�V����
void CSwitchDoor::Update()
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

}

// �`�揈��
void CSwitchDoor::Render()
{
	mpModel->Render(Matrix());
}

// �X�C�b�`���S�ĉ�����Ă��邩�ǂ���
bool CSwitchDoor::IsSwitchOn() const
{
	return false;
}
