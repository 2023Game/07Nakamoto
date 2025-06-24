#include "CSwitch.h"
#include "CColliderMesh.h"
#include "CStand.h"

#define PRESSED_OFFSET_POS 0.8f	// ������Ă���Ƃ���Y���W

// �R���X�g���N�^
CSwitch::CSwitch(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpPushedObject(nullptr)
	, mSwitch(false)
	, mDefaultPos(pos)
	, mOffSetPos(mDefaultPos)
	, mElapsedTime(0.0f)
{
	mpButtonModel = CResourceManager::Get <CModel>("Button");
	mpButtonColMesh = new CColliderMesh(this, ELayer::eSwitch, mpButtonModel, true);

	mpButtonColMesh->SetCollisionTags({ ETag::eCat,});
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
CSwitch::~CSwitch()
{
	SAFE_DELETE(mpButtonColMesh);
}

// �{�^����������Ă��邩�ǂ���
bool CSwitch::IsSwitchOn()
{
	return mSwitch;
}

// �Փˏ���
void CSwitch::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eCat)
	{
		if (self == mpButtonColMesh)
		{
			// ����Ă���I�u�W�F�N�g��ݒ�
			mpPushedObject = other->Owner();
			mElapsedTime = 0.0f;	// �o�ߎ��Ԃ����Z�b�g
			mSwitch = true;			// �X�C�b�`���I��
		}
	}
}

// �X�V
void CSwitch::Update()
{
	// �X�C�b�`�̏�ɏ���Ă���I�u�W�F�N�g�����݂��Ȃ��Ȃ���
	if (mpPushedObject == nullptr)
	{
		// ��莞�Ԍ�ɃX�C�b�`���I�t�ɂ���
		if (mElapsedTime < 0.25f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			mSwitch = false;
		}
	}
	// �X�C�b�`�𓥂�ł���I�u�W�F�N�g�̃|�C���^����ɂ���
	mpPushedObject = nullptr;

	CDebugPrint::Print("�X�C�b�`�F%s\n", mSwitch ? "�I��" : "�I�t");
}

// �`��
void CSwitch::Render()
{
	CMatrix m = Matrix();
	// �X�C�b�`��������Ă���Ƃ������A�������񂾈ʒu�Ɉړ�
	// �i���f���f�[�^�̍��W�����Y�����j
	if (mSwitch)
	{
		CMatrix transMtx;
		transMtx.Translate(0.0f, -PRESSED_OFFSET_POS, 0.0f);
		m = m * transMtx;
	}

	mpButtonModel->Render(m);
}
