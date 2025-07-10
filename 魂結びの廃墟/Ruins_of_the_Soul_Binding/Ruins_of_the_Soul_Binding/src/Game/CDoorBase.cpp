#include "CDoorBase.h"
#include "CNavManager.h"

#define DEFAULT_TEXT_PATH_OPEN		"UI\\Interact\\open.png"
#define DEFAULT_TEXT_PATH_CLOSE		"UI\\Interact\\close.png"

#define HP 60			// 体力

// コンストラクタ
CDoorBase::CDoorBase(const CVector& pos, const CVector& angle,
	std::string modelName, std::string colName)
	: mIsOpened(false)
	, mIsPlaying(false)
	, mpOwner(nullptr)
	, mpRoom(nullptr)
{
	Position(pos);
	Rotation(angle);

	// 扉のモデルデータの取得
	mpDoor = CResourceManager::Get<CModel>(modelName);
	// 扉のコライダーデータの取得
	mpDoorCol = CResourceManager::Get<CModel>(colName);

	// 扉のコライダー生成
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpDoorColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractSearch,
			ELayer::eBreakableSearch,
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
CDoorBase::~CDoorBase()
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
void CDoorBase::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ドアの開閉状態が切り替わった時に呼び出す関数のポインタを設定
void CDoorBase::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// どの部屋の扉か設定
void CDoorBase::SetRoom(CRoom* room)
{
	mpRoom = room;
}

// どの部屋の扉か返す
CRoom* CDoorBase::GetRoom() const
{
	return mpRoom;
}

// 調べる内容のテキスト画像のパスを返す
std::string CDoorBase::GetInteractTextPath() const
{
	// 閉まっているか
	if (mIsOpened)
	{
		// 閉まっていたら
		return DEFAULT_TEXT_PATH_CLOSE;
	}
	else
	{
		// 開いていたら
		return DEFAULT_TEXT_PATH_OPEN;
	}
}

// ドアが開いているかどうか
bool CDoorBase::IsOpened() const
{
	return mIsOpened;
}

// 調べられる状態かどうか
bool CDoorBase::CanInteract() const
{
	if (!mInteract) return false;
	if (mIsPlaying) return false;

	return true;
}

// 調べる
void CDoorBase::Interact()
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
void CDoorBase::Death()
{
	Kill();
}

// 衝突判定
bool CDoorBase::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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

// 描画処理
void CDoorBase::Render()
{
	mpDoor->Render(Matrix());
}
