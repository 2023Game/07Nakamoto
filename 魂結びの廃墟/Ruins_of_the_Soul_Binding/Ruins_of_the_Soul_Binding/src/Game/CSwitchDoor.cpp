#include "CSwitchDoor.h"
#include "CColliderMesh.h"
#include "CSwitch.h"
#include "CNavNode.h"
#include "CNavManager.h"

// コンストラクタ
CSwitchDoor::CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
	, mpOwner(this)
{
	mClosePos = pos;
	mOpenPos = openPos;
	Position(mIsOpened ? mOpenPos : mClosePos);

	Rotation(angle);

	mpModel = CResourceManager::Get<CModel>("");

	CModel* mesh = CResourceManager::Get<CModel>("");
	mpColliderMesh = new CColliderMesh(this, ELayer::eSwitch, mesh, true);
	mpColliderMesh->SetCollisionTags({ ETag::ePlayer, ETag::eCat, ETag::eEnemy });
	mpColliderMesh->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eEnemy ,
		}
	);
}

// デストラクタ
CSwitchDoor::~CSwitchDoor()
{
	// コライダーの削除
	if (mpColliderMesh != nullptr)
	{
		// 経路探索管理クラスが存在したら、遮蔽物リストからコライダーをを取り除く
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			navMgr->RemoveCollider(mpColliderMesh);
		}
		SAFE_DELETE(mpColliderMesh);
	}
}

// 接続するスイッチを追加
void CSwitchDoor::AddSwitch(CSwitch* sw)
{
	mpSwitches.push_back(sw);
}

bool CSwitchDoor::IsOpened() const
{
	return mIsOpened;
}


bool CSwitchDoor::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	// 扉のオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	return isHit;
}

// 更新処理
void CSwitchDoor::Update()
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

}

// 描画処理
void CSwitchDoor::Render()
{
	mpModel->Render(Matrix());
}

// スイッチが全て押されているかどうか
bool CSwitchDoor::IsSwitchOn() const
{
	return false;
}
