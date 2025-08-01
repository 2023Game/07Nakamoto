#include "CPressAndHoldSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"
#include "CNavManager.h"

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

	// �o�H�T���p�̎Օ����`�F�b�N�̃R���C�_�[�ɁA���̃R���C�_�[��o�^
	CNavManager::Instance()->AddCollider(mpButtonColMesh);

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
void CPressAndHoldSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eCat)
	{
		if (self == mpButtonColMesh)
		{
			mpPushedObject = other->Owner();
			// �O��̃t���[���ł��G��Ă����I�u�W�F�N�g�ł���΁A�������Ȃ�
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
	// �X�C�b�`�𓥂�ł���I�u�W�F�N�g�̃|�C���^����ɂ���
	mpPushedObject = nullptr;
}

// �`��
void CPressAndHoldSwitch::Render()
{
	CMatrix m = Matrix();
	if (mSwitch)
	{
		CMatrix transMtx;
		transMtx.Translate(0.0f, -PRESSED_OFFSET_POS, 0.0f);
		m = m * transMtx;
	}

	mpButtonModel->Render(m);
}
