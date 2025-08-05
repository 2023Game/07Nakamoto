#include "CFireball.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// コンストラクタ
CFireball::CFireball(float speed, float dist)
	: CObjectBase(ETag::ePlayer, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
{
	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Fireball");

	// コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		7.0f
	);

	// 敵とフィールドと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::eEnemy, ETag::eField });
	mpCollider->SetCollisionLayers({ ELayer::eEnemy, ELayer::eFloor , ELayer::eWall });
}

// デストラクタ
CFireball::~CFireball()
{
	// コライダー削除
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CFireball::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 敵に衝突した
	if (other->Layer() == ELayer::eEnemy)
	{
		// 敵にダメージを与える
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(10, this);
			// 自身を削除
			Kill();
		}
	}
	if (other->Layer() == ELayer::eWall || other->Layer() == ELayer::eFloor)
	{
		// 何かにぶつかったら、自身を削除
		Kill();
	}
}

// 更新処理
void CFireball::Update()
{
	// このフレームで移動距離を求める
	float moveDist = mMoveSpeed * Times::DeltaTime() * 0.5f;

	// 移動距離分、移動させる
	CVector pos = Position();
	pos += VectorZ() * moveDist;
	Position(pos);

	// 移動した距離を加算
	mCurrDist += moveDist;
	// 移動出来る距離を超えたら、自身を削除
	if (mCurrDist >= mMoveDist)
	{
		Kill();
	}
}

// 描画処理
void CFireball::Render()
{
	mpModel->Render(Matrix());
}

