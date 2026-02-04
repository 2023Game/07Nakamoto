#include "CColTriangle.h"
#include "Primitive.h"

// コンストラクタ
CColTriangle::CColTriangle(const CVector& v0, const CVector& v1, const CVector& v2, const CColor& defaultColor,
	bool isKinematic, float weight)
	: CColBase(defaultColor)
{
	mVertices[0] = v0;
	mVertices[1] = v1;
	mVertices[2] = v2;

	mpCollider = new CColliderTriangle
	(
		this, ELayer::eTest,
		mVertices[0], mVertices[1], mVertices[2],
		isKinematic, weight
	);
	mpCollider->SetCollisionLayers({ ELayer::eTest });
}

// デストラクタ
CColTriangle::~CColTriangle()
{
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CColTriangle::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	CColBase::Collision(self, other, hit);

	//if (mIsSelect)
	{
		//CColliderSphere* cs = dynamic_cast<CColliderSphere*>(other);
		//if (cs != nullptr)
		{
			Position(Position() + hit.adjust * hit.weight);
		}
	}
}

// 描画
void CColTriangle::Render()
{
	CMatrix m = mpCollider->Matrix();
	Primitive::DrawWireTriangle
	(
		m * mVertices[0],
		m * mVertices[1],
		m * mVertices[2],
		GetColor()
	);
}
