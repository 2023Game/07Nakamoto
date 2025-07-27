#include "CItemObj.h"
#include "CColliderSphere.h"
#include "CInventory.h"
#include "CImage3D.h"
#include "Maths.h"

#define INTERACT_TEXT_PATH "UI\\Interact\\pickup.png"
#define WORLD_UNIT_PER_PIXEL 80.0f

// 1秒間で回転する角度
#define ROTATE_SPEED 90.0f

// 拡縮アニメーションの周期
#define SCALING_TIME 2.0f
// 拡縮アニメーション時のスケール最小値
#define SCALING_MIN 0.75f
// 拡縮アニメーション時のスケール最大値
#define SCALING_MAX 1.25f

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

	// ランダムで所為の回転角度を求める
	float angleZ = Math::Rand(0.0f, 360.0f);
	mpItemImage->Rotation(0.0f, 0.0f, angleZ);
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
	// 時間経過に合わせて拡縮アニメーションを再生
	float alpha = fmodf(Times::Time(), SCALING_TIME) / SCALING_TIME;
	float t = (sinf(M_PI * 2.0f * alpha) + 1.0f) * 0.5f;
	float scale = Math::Lerp(SCALING_MIN, SCALING_MAX, t);
	mpItemImage->Scale(scale,scale,scale);

	// Z軸回転
	mpItemImage->Rotate(0.0f, 0.0f, ROTATE_SPEED * Times::DeltaTime());

	mpItemImage->Position(Position());
	mpItemImage->Update();
}

// 描画処理
void CItemObj::Render()
{
	mpItemImage->Render();
}

