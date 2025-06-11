#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CWarrok.h"
#include "CInteractObjectManager.h"

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
		this, ELayer::eDemon,
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
	mpCollider->Position(pos);

	mHp = HP;
	Position(pos);

	//CDemonPowerManager::Instance()->AddDemonPower(this);
	
}

// デストラクタ
CDemonPower::~CDemonPower()
{
	// コライダーを削除
	if (mpCollider != nullptr)
	{
		// 妖力の源の管理リストから取り除く
		CDemonPowerManager* dpManager = CDemonPowerManager::Instance();
		if (dpManager != nullptr)
		{
			dpManager->RemovePower(this);
		}
		SAFE_DELETE(mpCollider);
	}
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
		CWarrok::Instance()->PowerDown();
	}
}

// 描画処理
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
