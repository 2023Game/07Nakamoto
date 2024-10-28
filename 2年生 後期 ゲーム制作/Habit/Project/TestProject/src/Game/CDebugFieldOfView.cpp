#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "glut.h"

// �R���X�g���N�^
CDebugFieldOfView::CDebugFieldOfView(CObjectBase* owner, float fovAngle, float fovLength)
	: CObjectBase(ETag::eDebug, ETaskPriority::eDebugFov)
	, mpOwner(owner)
	, mFovAngle(fovAngle)
	, mFovLength(fovLength)
{
}

// �f�X�g���N�^
CDebugFieldOfView::~CDebugFieldOfView()
{
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// ����͈͂�\������Ώۂ�ݒ�
void CDebugFieldOfView::SetOwner(CObjectBase* owner)
{
}

// ����͈͂�ݒ�
void CDebugFieldOfView::Set(float angle, float length)
{
}

// �X�V
void CDebugFieldOfView::Update()
{
}

// �`��
void CDebugFieldOfView::Render()
{
	// �\���Ώۂ����Ȃ��ꍇ�́A�`�揈�����s��Ȃ�
	if (mpOwner == nullptr) return;

	// �f�v�X�e�X�g�I�t
	glDisable(GL_DEPTH_TEST);

	// ����͈͂𔼓����ɂ���
	mColor.A(0.5f);

	// ����͈͂̐�`��`��
	Primitive::DrawSector
	(
		mpOwner->Matrix(),		// �\���Ώۂ̍s��
		-mFovAngle, mFovAngle,	// -����p�x �` +����p�x�͈̔͂�ݒ�
		mFovLength,				// ���싗��
		mColor					// ��`�̐F
	);

	// �f�v�X�e�X�g�̏�Ԃ�߂�
	glEnable(GL_DEPTH_TEST);
}
