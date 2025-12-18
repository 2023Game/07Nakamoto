#include "CSlash.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// コンストラクタ
CSlash::CSlash(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist)
	: CObjectBase(ETag::eSlash, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Slash");
	mpModel->SetupEffectSettings();

	// コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		3.0f
	);
	// プレイヤーとフィールドと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::eField ,ETag::eEnemy});
	mpCollider->SetCollisionLayers({ ELayer::eFloor, ELayer::eWall, ELayer::eEnemy });

}

// デストラクタ
CSlash::~CSlash()
{
	// コライダー削除
	SAFE_DELETE(mpCollider);
}

// 衝突判定
void CSlash::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 敵に衝突した
	if (other->Layer() == ELayer::eEnemy)
	{
		// 敵にダメージを与える
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(1, this);
		}
	}

	// 何かにぶつかったら、自身を削除
	Kill();
}

// 更新
void CSlash::Update()
{
	CVector move = mMoveSpeed * Times::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// 描画
void CSlash::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}