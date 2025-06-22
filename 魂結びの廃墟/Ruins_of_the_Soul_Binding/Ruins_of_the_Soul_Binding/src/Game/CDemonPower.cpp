#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CBoss.h"
#include "CInteractObjectManager.h"
#include "CGaugeUI3D.h"

#define HP 10	// 耐久力
#define GAUGE_OFFSET_Y 10.0f
#define OFFSET_POS CVector(0.0f, 1.5f, 0.0f)	// コライダーのオフセット

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
	mpCollider->Position(OFFSET_POS);

	mMaxHp = HP;
	mHp = mMaxHp;
	Position(pos);

	// HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	CDemonPowerManager::Instance()->AddDemonPower(this);
	
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

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}
}

// 調べる
void CDemonPower::Interact()
{

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
		CBoss::Instance()->PowerDown();
	}

	// HPゲージが存在すれば、
	if (mpHpGauge != nullptr)
	{
		// HPゲージを更新
		mpHpGauge->Position(Position() + mGaugeOffsetPos);
		mpHpGauge->SetMaxPoint(mMaxHp);
		mpHpGauge->SetCurrPoint(mHp);
	}

}

// 描画処理
void CDemonPower::Render()
{
	mpModel->Render(Matrix());
}
