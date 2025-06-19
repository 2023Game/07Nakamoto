#include "CSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// ������Ă���Ƃ��̍��W

// �R���X�g���N�^
CSwitch::CSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eObject, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy });
	mpButtonColMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eEnemy ,
		}
	);

	Position(mDefaultPos);
	mOffSetPos.Y(mDefaultPos.Y() - PRESSED_OFFSET_POS);

	// �X�C�b�`�̑�𐶐�
	new CStand(pos);
}

// �f�X�g���N�^
CSwitch::~CSwitch()
{
	if (mpButtonColMesh != nullptr)
	{
		SAFE_DELETE(mpButtonColMesh);
	}
}

// �{�^����������Ă��邩�ǂ���
bool CSwitch::IsSwitchOn()
{
	return mSwitch;
}

void CSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::ePlayer)
	{
		if (self == mpButtonColMesh)
		{
			mSwitch = true;
		}
	}
}

// �X�V
void CSwitch::Update()
{
	if (!mSwitch)
	{
		Position(mDefaultPos);
	}
	else
	{
		Position(mOffSetPos);
	}

	mSwitch = false;
}

// �`��
void CSwitch::Render()
{
	mpButtonModel->Render(Matrix());
}
