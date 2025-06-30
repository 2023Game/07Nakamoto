#include "COpeningDoor.h"
#include "CNavManager.h"

#define HP 60			// 体力
#define DOOR_WIDTH 11.0f	// ドアの幅

#define UI_OFFSET_POS_Y	10.0f
#define UI_OFFSET_POS_Z	 -4.95f

// コンストラクタ
COpeningDoor::COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle)
	: mIsOpened(false)
	, mAnimTime(1.0f)
	, mElapsedTime(0.0f)
	, mIsPlaying(false)
	, mpOwner(nullptr)
{
	mOpenAngle = openAngle;
	mCloseAngle = angle;
	Rotate(mIsOpened ? mOpenAngle : mCloseAngle);
	Position(pos);

	// UIの座標を設定
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;

	// 扉のモデルデータの取得
	mpDoor = CResourceManager::Get<CModel>("OpeningDoor");
	// 扉のコライダーデータの取得
	mpDoorCol = CResourceManager::Get<CModel>("OpeningDoorCol");

	// 扉のコライダー生成
	mpDoorColliderMesh = new CColliderMesh(this, ELayer::eDoor, mpDoorCol, true);
	mpDoorColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpDoorColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
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
COpeningDoor::~COpeningDoor()
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
void COpeningDoor::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ドアの開閉状態が切り替わった時に呼び出す関数のポインタを設定
void COpeningDoor::SetOnChangeFunc(std::function<void()> func)
{
	mOnChangeFunc = func;
}

// ドアが開いているかどうか
bool COpeningDoor::IsOpened() const
{
	return mIsOpened;
}

// 調べられる状態かどうか
bool COpeningDoor::CanInteract() const
{
	// ドアを開閉している途中なら、調べられない
	if (mIsPlaying) return false;

	return true;
}

// 調べる
void COpeningDoor::Interact()
{
	// TODO:鍵が閉まっていたら、開かない

	// 閉まっている
	if (!mIsOpened)
	{
		mIsOpened = true;
		mIsPlaying = true;
	}
	// 開いている
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

// 調べるときに参照するオブジェクトの位置
CVector COpeningDoor::GetInteractPos() const
{
	// 開き戸は回転軸が原点なので、
	// 回転軸からドアの幅の半分だけズラした位置を調べる位置とする
	return Position() + VectorX() * DOOR_WIDTH * 0.5f;
}

// 壊れた時の処理
void COpeningDoor::Death()
{
	Kill();
}

// レイとフィールドオブジェクトの衝突判定
bool COpeningDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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
void COpeningDoor::Update()
{
	if (mIsPlaying)
	{
		CVector strtAngle = mIsOpened ? mCloseAngle : mOpenAngle;
		CVector endAngle = mIsOpened ? mOpenAngle : mCloseAngle;

		// 時間経過による開閉アニメーション
		if (mElapsedTime < mAnimTime)
		{
			float per = mElapsedTime / mAnimTime;
			CVector angle = CVector::Lerp(strtAngle, endAngle, per);
			Rotation(CQuaternion(angle));
			mElapsedTime += Times::DeltaTime();
		}
		// アニメーション時間を経過した
		else
		{
			Rotation(CQuaternion(endAngle));
			mElapsedTime = 0.0f;
			mIsPlaying = false;
		}
	}

	// UIの座標を設定
	mOffSetPos = CVector(0.0f, UI_OFFSET_POS_Y, 0.0f);
	mOffSetPos -= VectorX() * UI_OFFSET_POS_Z;
}

// 描画処理
void COpeningDoor::Render()
{
	mpDoor->Render(Matrix());
}
