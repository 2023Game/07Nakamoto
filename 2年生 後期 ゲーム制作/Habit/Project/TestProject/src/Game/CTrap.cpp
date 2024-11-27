#include "CTrap.h"
#include "Primitive.h"
#include "CImage.h"

// コンストラクタ
CTrap::CTrap(const CVector& pos)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mPosition(pos)
	, mColor(1.0f, 0.0f, 0.0f, 1.0f)
	, mpOwner(nullptr)
{
	mpTexture = CResourceManager::Get<CTexture>("spider_web");
}

// デストラクタ
CTrap::~CTrap()
{
	// 持ち主が存在する場合は、
	if (mpOwner != nullptr)
	{
		// 持ち主に自分が削除されたことを伝える
		mpOwner->DeleteObject(this);
	}
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
