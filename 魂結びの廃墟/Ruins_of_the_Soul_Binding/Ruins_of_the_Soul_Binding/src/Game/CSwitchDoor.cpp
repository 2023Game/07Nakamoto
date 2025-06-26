#include "CSwitchDoor.h"
#include "CSwitch.h"
#include "CNavManager.h"

//コンストラクタ
CSwitchDoor::CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mpOwner(nullptr)
{
	mOpenPos = openPos;
	mClosePos = pos;
	Position(mIsOpened ? mOpenPos : mClosePos);
	Rotate(angle);

	// 扉のモデルデータの取得
	mpModel = CResourceManager::Get<CModel>(modelName);
	// 扉のコライダーデータの取得
	CModel* modelCol = CResourceManager::Get<CModel>(colName);

	// 扉のコライダー生成
	mpModelColliderMesh = new CColliderMesh(this, ELayer::eDoor, modelCol, true);
	mpModelColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpModelColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eEnemy ,
		}
	);

	// 経路探索用の遮蔽物チェックのコライダーに、扉のコライダーを登録
	CNavManager::Instance()->AddCollider(mpModelColliderMesh);
}

// デストラクタ
CSwitchDoor::~CSwitchDoor()
{
	// コライダーの削除
	if (mpModelColliderMesh != nullptr)
	{
		// 経路探索管理クラスが存在したら、遮蔽物リストからドアのコライダーをを取り除く
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpModelColliderMesh);
		}
		delete mpModelColliderMesh;
		mpModelColliderMesh = nullptr;
	}

	// 持ち主が存在する場合は、持ち主に自身が削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 接続するスイッチを追加
void CSwitchDoor::AddSwitch(CSwitch* sw)
{
	mpSwitches.push_back(sw);
}

// 持ち主を設定する
void CSwitchDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// 扉の開閉状態が切り替わった時に呼び出す関数のポインタを設定
void CSwitchDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// ドアが開いているかどうか
bool CSwitchDoor::IsOpened() const
{
	return mIsOpened;
}

// レイとフィールドオブジェクトの衝突判定
bool CSwitchDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	// 扉のオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpModelColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;
}

// 更新処理
void CSwitchDoor::Update()
{
	if (mIsPlaying)
	{
		CVector startPos = mIsOpened ? mClosePos : mOpenPos;
		CVector endPos = mIsOpened ? mOpenPos : mClosePos;

		// 時間経過による開閉アニメーション
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector pos = CVector::Lerp(startPos, endPos, per);
			Position(pos);
			mElapsedTime += Times::DeltaTime();
		}
		// アニメーション時間を経過した
		else
		{
			Position(endPos);
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}
	else
	{
		bool isSwitchOn = IsAllSwitchOn();

		if (isSwitchOn && !mIsOpened)
		{
			mIsOpened = true;
			mIsPlaying = true;
		}
		else if (!isSwitchOn && mIsOpened)
		{
			mIsOpened = false;
			mIsPlaying = true;
		}
	}
}

// スイッチが全て押されているかどうか
bool CSwitchDoor::IsAllSwitchOn() const
{
	for (CSwitch* sw : mpSwitches)
	{
		if (!sw->IsSwitchOn()) return false;
	}
	return true;
}

// 描画処理
void CSwitchDoor::Render()
{
	mpModel->Render(Matrix());
}


