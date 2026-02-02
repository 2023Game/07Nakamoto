#include "CEscapeArea.h"
#include "CColliderSphere.h"
#include "CSceneManager.h"
#include "CModel.h"
#include "CField.h"
#include "CGameData.h"

#define CLEAR_FLOOR 1

#define SPEED 1.0f
#define MAX_HIGHT 8
#define MIN_HIGHT 3

// コンストラクタ
CEscapeArea::CEscapeArea(const CVector& pos, const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eField, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mStateStep(0)
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
		// 一度触れたらコライダーをオフにする
		mpCollider->SetEnable(false);

		// クリアしたのが5階層目より下の場合
		if (CGameData::floorNum < CLEAR_FLOOR)
		{
			CGameData::floorNum++;
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// 全階層クリアした場合
		else
		{
			// リザルトシーンを読み込む
			CSceneManager::Instance()->LoadScene(EScene::eResult);
		}
	}
}

// 更新処理
void CEscapeArea::Update()
{
	CVector pos = Position();
	float nextY = pos.Y();

	switch (mStateStep)
	{
	case 0: // 上昇
		nextY += SPEED * Times::DeltaTime();
		if (nextY >= MAX_HIGHT) {
			nextY = MAX_HIGHT;
			mStateStep = 1; // 次のフレームから下降
		}
		break;
	case 1: // 下降
		nextY -= SPEED * Times::DeltaTime();
		if (nextY <= MIN_HIGHT) {
			nextY = MIN_HIGHT;
			mStateStep = 0; // 次のフレームから上昇
		}
		break;
	}

	Position(CVector(pos.X(), nextY, pos.Z()));
}

// 描画処理
void CEscapeArea::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
