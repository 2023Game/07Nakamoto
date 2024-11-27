#include "CTrap.h"
#include "Primitive.h"
#include "CImage.h"

// �R���X�g���N�^
CTrap::CTrap(const CVector& pos)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mPosition(pos)
	, mColor(1.0f, 0.0f, 0.0f, 1.0f)
	, mpOwner(nullptr)
{
	mpTexture = CResourceManager::Get<CTexture>("spider_web");
}

// �f�X�g���N�^
CTrap::~CTrap()
{
	// �����傪���݂���ꍇ�́A
	if (mpOwner != nullptr)
	{
		// ������Ɏ������폜���ꂽ���Ƃ�`����
		mpOwner->DeleteObject(this);
	}
}

// 㩂�ݒu������W��Ԃ�
CVector CTrap::GetOffsetPos() const
{
	return CVector(mPosition);
}

// �X�V
void CTrap::Update()
{
}

// �`��
void CTrap::Render()
{
	CMatrix m;
	m.Translate(GetOffsetPos());
	Primitive::DrawSphere(m, 2.0f, mColor);
}
