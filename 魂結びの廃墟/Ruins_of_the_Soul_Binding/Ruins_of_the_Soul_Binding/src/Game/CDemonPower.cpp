#include "CDemonPower.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CWarrok.h"

#define HP 10	// 耐久力

// コンストラクタ
CDemonPower::CDemonPower(const CVector& pos)
	:mBroken(false)
{
	// モデルデータの取得
	mpModel = CResourceManager::Get<CModel>("DemonPower");

	// コライダーを設定
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		4.0f, true
	);

	mpCollider->SetCollisionTags
	( 
		{
			ETag::ePlayer,
			ETag::eEnemy,
		}
	);
	mpCollider->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy
		}
	);

	mHp = HP;
	Position(pos);

}

// デストラクタ
CDemonPower::~CDemonPower()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// 調べる
void CDemonPower::Interact()
{
	mHp -= 5;
}

// 破壊されたか
bool CDemonPower::BreakPower()
{
	return mBroken;
}

// 更新処理
void CDemonPower::Update()
{
	if (mHp <= 0)
	{
		Kill();
	}
}

// 描画処理
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
