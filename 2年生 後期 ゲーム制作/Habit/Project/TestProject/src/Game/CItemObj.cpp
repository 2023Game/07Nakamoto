#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"

// コンストラクタ
CItemObj::CItemObj(ItemType type)
	: CInteractObject(ETaskPriority::eItem,0,ETaskPauseType::eGame)
	, mItemTyope(type)
	, mpItemData(nullptr)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// 指定されたアイテムの種類からアイテムデータを取得
	bool success = Item::GetItemData(mItemTyope, &mpItemData);
	// アイテムデータが存在しなかったら、自身を削除
	if (!success)
	{
		Kill();
		return;
	}

	// アイテムデータのモデルデータを取得
	mpModel = CResourceManager::Get<CModel>(mpItemData->modelPath);

	// コライダーを作成
	CreateCollider();

	// 調べるときに表示するテキストを設定
	mInteractStr = "拾う";
}

// デストラクタ
CItemObj::~CItemObj()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// コライダー作成(継承先で上書き可)
void CItemObj::CreateCollider()
{
	// コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		10.0f, true
	);

	// プレイヤーと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eInteractSearch });

}

// 調べられる状態かどうか
bool CItemObj::CanInteract() const
{
	if (IsKill()) return false;

	return true;
}

// 調べる
void CItemObj::Interact()
{
	//インベントリに追加
	CInventory::Instance()->AddItem(mItemTyope, 1);

	Kill();

}

// 更新処理
void CItemObj::Update()
{
}

// 描画処理
void CItemObj::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}

