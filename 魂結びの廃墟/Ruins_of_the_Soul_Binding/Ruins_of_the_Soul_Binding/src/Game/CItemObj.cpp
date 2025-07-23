#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"
#include "CImage3D.h"

#define INTERACT_TEXT_PATH "UI\\Interact\\pickup.png"
#define WORLD_UNIT_PER_PIXEL 100.0f
#define MIN_SIZ		CVector2(50.0f,50.0f)

// コンストラクタ
CItemObj::CItemObj(ItemType type)
	: CInteractObject(ETaskPriority::eItem, 0, ETaskPauseType::eGame)
	, mItemTyope(type)
	, mpItemData(nullptr)
	, mpItemImage(nullptr)
	, mpCollider(nullptr)
{
	// 調べるテキストの画像を読み込む
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH, INTERACT_TEXT_PATH);

	// 指定されたアイテムの種類からアイテムデータを取得
	bool success = Item::GetItemData(mItemTyope, &mpItemData);
	// アイテムデータが存在しなかったら、自身を削除
	if (!success)
	{
		Kill();
		return;
	}

	// アイテムデータのモデルデータを取得
	//mpModel = CResourceManager::Get<CModel>(mpItemData->modelPath);

	mpItemImage = new CImage3D
	{
		"Item\\2D\\Light_of_the_item.png",
		ETag::eItem,
		ETaskPriority::eItem,0,
		ETaskPauseType::eGame,
		false,false
	};
	mpItemImage->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpItemImage->SetBillboard(true);

	// コライダーを作成
	CreateCollider();

	// 調べるときに表示するテキストを設定
	mInteractStr = "拾う";
}

// デストラクタ
CItemObj::~CItemObj()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpItemImage);
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

// 調べる内容のテキスト画像のパスを返す
std::string CItemObj::GetInteractTextPath() const
{
	return INTERACT_TEXT_PATH;
}

// 調べるUIを表示する座標を返す
CVector CItemObj::GetInteractUIPos() const
{
	return Position() + CVector(0.0f, 10.0f, 0.0f);
}

// 更新処理
void CItemObj::Update()
{
	mpItemImage->SetSize(mpItemImage->GetSize() - CVector2(1.0f, 1.0f) * Times::DeltaTime());

	mpItemImage->Rotation(mpItemImage->Rotation());

	mpItemImage->Position(Position());
	mpItemImage->Update();
}

// 描画処理
void CItemObj::Render()
{
	mpItemImage->Render();
}

