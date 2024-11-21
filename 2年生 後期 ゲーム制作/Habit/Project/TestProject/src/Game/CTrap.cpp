#include "CTrap.h"
#include "Primitive.h"

// �R���X�g���N�^
CTrap::CTrap(const CVector& pos)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mPosition(pos)
	, mColor(1.0f, 0.0f, 0.0f, 1.0f)
{
	
}

// �f�X�g���N�^
CTrap::~CTrap()
{
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
