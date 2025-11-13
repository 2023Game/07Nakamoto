#include "CCrate.h"
#include "CColliderMesh.h"

CCrate::CCrate(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("Crate");

	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, mpModel, false);
	mpColliderMesh->SetCollisionTags({ ETag::eField,ETag::ePlayer,ETag::eEnemy });
	mpColliderMesh->SetCollisionLayers({ ELayer::eWall, ELayer::ePlayer,ELayer::eEnemy });

	Position(pos);
}

CCrate::~CCrate()
{
	SAFE_DELETE(mpColliderMesh);
}

// 衝突判定
void CCrate::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderMesh)
	{
		// プレイヤーと衝突した場合
		if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

void CCrate::Update()
{
}

void CCrate::Render()
{
	mpModel->Render(Matrix());
}
