#include "CDoor.h"
#include "CNavManager.h"

//#define MOVE_POS 9.9f	// 移動距離
//#define MOVE_TIME 5.0f	// 移動時間
#define HP 60			// 体力

// コンストラクタ
CDoor::CDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName , std::string colName)
	: mIsOpened(false)
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
	mpDoor = CResourceManager::Get<CModel>(modelName);
	// 扉のコライダーデータの取得
	mpDoorCol = CResourceManager::Get<CModel>(colName);

	// 扉のコライダー生成
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eEnemy});
	mpDoorColliderMesh->SetCollisionLayers
	(
		{ 
			ELayer::ePlayer,
			ELayer::eInteractSearch,
			ELayer::eEnemy ,
			ELayer::eAttackCol
		}
	);

	// 経路探索用の遮蔽物チェックのコライダーに、扉のコライダーを登録
	CNavManager::Instance()->AddCollider(mpDoorColliderMesh);

	mHp = HP;

	mInteractStr = "閉まっている";
}

// デストラクタ
CDoor::~CDoor()
{
	// コライダーの削除
	if (mpDoorColliderMesh != nullptr)
	{
		// 経路探索管理クラスが存在したら、遮蔽物リストからドアのコライダーをを取り除く
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpDoorColliderMesh);
		}
		SAFE_DELETE(mpDoorColliderMesh);
	}

	// 持ち主が存在する場合は、持ち主に自身が削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定する
void CDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ドアの開閉状態が切り替わった時に呼び出す関数のポインタを設定
void CDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// ドアが開いているかどうか
bool CDoor::IsOpened() const
{
	return mIsOpened;
}

// 調べる
void CDoor::Interact()
{
	// 閉まっている
	if (!mIsOpened)
	{
		mIsOpened = true;
		mIsPlaying = true;
	}
	else if (mIsOpened)
	{
		mIsOpened = false;
		mIsPlaying = true;
	}

	// 切り替え時に呼び出す関数が設定されていたら、
	// その関数を呼び出す
	if (mOnChangeFunc != nullptr)
	{
		mOnChangeFunc();
	}
	
	mInteractStr = mIsOpened ? "閉まっている" : "開いている";

}

// 壊れた時の処理
void CDoor::Death()
{
	Kill();
}

// 衝突判定
bool CDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	// 扉のオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpDoorColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;
}

// 更新処理
void CDoor::Update()
{
	if (mIsPlaying)
	{
		mInteract = false;
		// 扉を開くアニメーション
		if (mIsOpened)
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mClosePos, mOpenPos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mOpenPos);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
				mInteract = true;
			}
		}
		// 扉を閉じるアニメーション
		else
		{
			if (mElapsedTime < mAnimTime)
			{
				float per = mElapsedTime / mAnimTime;
				CVector pos = CVector::Lerp(mOpenPos, mClosePos, per);
				Position(pos);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				Position(mClosePos);
				mElapsedTime = 0.0f;
				mIsPlaying = false;
			}
		}
	}
	// 開閉中ではない
	else
	{
		mInteract = true;
	}		
}

// 描画処理
void CDoor::Render()
{
	mpDoor->Render(Matrix());
}
