#include "CPressAndHoldSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// ������Ă���Ƃ���Y���W

// �R���X�g���N�^
CPressAndHoldSwitch::CPressAndHoldSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpLastPushedObject(nullptr)
	, mpPushedObject(nullptr)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
	, mElapsedTime(0.0f)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eSwitch, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::eCat, });
	mpButtonColMesh->SetCollisionLayers
	(
		{
			ELayer::eCat,
		}
	);

	Position(mDefaultPos);
	mOffSetPos.Y(mDefaultPos.Y() - PRESSED_OFFSET_POS);

	// �X�C�b�`�̑�𐶐�
	new CStand(pos);

}

// �f�X�g���N�^
CPressAndHoldSwitch::~CPressAndHoldSwitch()
{
	SAFE_DELETE(mpButtonColMesh);
}

// �{�^����������Ă��邩�ǂ���
bool CPressAndHoldSwitch::IsSwitchOn()
{
	return mSwitch;
}

// �{�^���̃I���I�t�؂�ւ�
void CPressAndHoldSwitch::ChangeSwith()
{
	mSwitch = !mSwitch;
}

// �Փˏ���
bool CPressAndHoldSwitch::IsPushedObject(CObjectBase* obj) const
{
}

// �Փˏ���
void CPressAndHoldSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eCat)
	{
		if (self == mpButtonColMesh)
		{
			mpPushedObject = other->Owner();
			// �O��̂ւ�[�ނł��G��Ă����I�u�W�F�N�g�ł���΁A�������Ȃ�
			if (mpPushedObject == mpLastPushedObject)	return;

			mpLastPushedObject = mpPushedObject;
			// �X�C�b�`�̏�Ԃ�؂�ւ���
			ChangeSwith();

			mElapsedTime = 0.0f;
		}
	}
}

// �X�V
void CPressAndHoldSwitch::Update()
{
	// �O��̃t���[���ŃX�C�b�`�𓥂�ł���I�u�W�F�N�g�����݂��Ȃ������ꍇ
	if (mpPushedObject == nullptr)
	{
		if (mElapsedTime < 1.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// �O��ɃX�C�b�`�𓥂�ł���I�u�W�F�N�g�̃|�C���^��������
			mpLastPushedObject = nullptr;
		}
	}

	if (!mSwitch)
	{
		Position(mDefaultPos);
	}
	else
	{
		Position(mOffSetPos);
	}

	// �X�C�b�`�𓥂�ł���I�u�W�F�N�g�̃|�C���^����ɂ���
	mpPushedObject = nullptr;
}

// �`��
void CPressAndHoldSwitch::Render()
{
	mpButtonModel->Render(Matrix());
}
