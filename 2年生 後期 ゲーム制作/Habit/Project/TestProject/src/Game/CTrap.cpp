#include "CTrap.h"
#include "CColliderMesh.h"
#include "CCharaBase.h"
#include "CPlayer2.h"

#define SLOW 0.5f
#define TIME 3

CTrap::CTrap()
	: CObjectBase(ETag::eTrap, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// モデルデータの取得
	mpModel = CResourceManager::Get<CModel>("Spider_Web");

	// コライダーを作成
	mpCollider = new CColliderMesh
	(
		this, ELayer::eTrap,
		mpModel, true
	);
	// プレイヤーと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer , ETag::eEnemy});
	mpCollider->SetCollisionLayers({ ELayer::ePlayer , ELayer::eEnemy});

}

// デストラクタ
CTrap::~CTrap()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

void CTrap::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// プレイヤーに衝突した
	if (other->Layer() == ELayer::ePlayer)
	{
		// プレイヤーにダメージを与える
		CPlayer2* chara = dynamic_cast<CPlayer2*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeSlow(SLOW, TIME);
			// プレイヤーに当たったら、自身を削除
			Kill();
		}
	}
}

void CTrap::Update()
{
}

// 描画
void CTrap::Render()
{
	mpModel->Render(Matrix());
}
