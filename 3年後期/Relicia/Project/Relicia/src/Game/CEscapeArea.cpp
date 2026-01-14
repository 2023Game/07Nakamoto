#include "CEscapeArea.h"
#include "CColliderSphere.h"
#include "CSceneManager.h"
#include "CModel.h"

// コンストラクタ
CEscapeArea::CEscapeArea(const CVector& pos, const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eField, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{
	// クリアエリアのモデルデータを取得
	mpModel = CResourceManager::Get<CModel>("Escape");

	// クリアエリアのコライダーを作成
	mpCollider = new CColliderSphere(this, ELayer::eFloor, 10.0f, true);
	// プレイヤーとフィールドと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer });

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// デストラクタ
CEscapeArea::~CEscapeArea()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CEscapeArea::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// プレイヤーに衝突した
	if (other->Layer() == ELayer::ePlayer)
	{
		// リザルトシーンを読み込む
		CSceneManager::Instance()->LoadScene(EScene::eResult);
	}
}

// 描画処理
void CEscapeArea::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
