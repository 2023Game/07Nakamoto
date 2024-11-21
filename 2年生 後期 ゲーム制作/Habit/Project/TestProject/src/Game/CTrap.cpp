#include "CTrap.h"
#include "Primitive.h"

// コンストラクタ
CTrap::CTrap(const CVector& pos)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mPosition(pos)
	, mColor(1.0f, 0.0f, 0.0f, 1.0f)
{
	
}

// デストラクタ
CTrap::~CTrap()
{
}

// 罠を設置する座標を返す
CVector CTrap::GetOffsetPos() const
{
	return CVector(mPosition);
}

// 更新
void CTrap::Update()
{
}

// 描画
void CTrap::Render()
{
	CMatrix m;
	m.Translate(GetOffsetPos());
	Primitive::DrawSphere(m, 2.0f, mColor);
}
