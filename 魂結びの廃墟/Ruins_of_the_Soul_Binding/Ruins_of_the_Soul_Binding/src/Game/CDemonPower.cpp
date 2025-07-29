#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CBoss.h"
#include "CInteractObjectManager.h"
#include "CGaugeUI3D.h"

#define DEFAULT_TEXT_PATH "UI\\Interact\\break.png"
#define DEFAULT_KEY_PATH "UI\\BottonUI\\E.png"

#define HP 10	// 耐久力

#define KYE_UI_OFFSET_Y 12.0f	// キーのUIのオフセット
#define GAUGE_OFFSET_Y 10.0f	// HPゲージのオフセット
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
			ETag::eCat,
			ETag::eEnemy,
		}
	);
	mpCollider->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractSearch,
			ELayer::eEnemy
		}
	);
	mpCollider->Position(OFFSET_POS);

	mMaxHp = HP;
	mHp = mMaxHp;
	Position(pos);

	mOffSetPos.Y(KYE_UI_OFFSET_Y);

	// HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
	// ゲージのオフセット位置を設定
	mGaugeOffsetPos.Y(GAUGE_OFFSET_Y);

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

// 調べる内容のテキスト画像のパスを返す
std::string CDemonPower::GetInteractTextPath() const
{
	return DEFAULT_TEXT_PATH;
}

// 調べる内容のテキスト画像のパスを返す
std::string CDemonPower::GetInteractKeyPath() const
{
	return DEFAULT_KEY_PATH;
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
