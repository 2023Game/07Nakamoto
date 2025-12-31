#include "CCrystalObj.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CElementManager.h"

#define COLLIDER_RADIUS 10.0f

// コンストラクタ
CCrystalObj::CCrystalObj(ElementType type, CVector pos)
	: CObjectBase(ETag::eItem, ETaskPriority::eCrystal, 0, ETaskPauseType::eGame)
	, mElementType(type)
	, mpCrystalData(nullptr)
	, mpModel(nullptr)
	, mpCollider(nullptr)
{
	// 指定された属性の種類からクリスタルのデータを取得
	bool success = Crystal::GetCrystalData(mElementType, &mpCrystalData);
	// クリスタルのデータが存在しなかったら、自身を削除
	if (!success)
	{
		Kill();
		return;
	}
	// クリスタルのモデルデータを取得
	mpModel = CResourceManager::Get <CModel>("Crystal");

	// クリスタルの元のマテリアルを取得
	CMaterial* baseMat = mpModel->GetMaterial();
	// 元のマテリアルをコピーして新しいマテリアルを生成
	mpMaterial = new CMaterial(*baseMat);
	// 新しいマテリアルのテクスチャを読み込む
	std::string texPath = mpMaterial->DirPath() + mpCrystalData->texPath;
	mpMaterial->LoadTexture(texPath, texPath, false);

	// 球コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eCrystal,
		COLLIDER_RADIUS, true
	);
	// プレイヤーと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer });
	
	Position(pos);
}

// デストラクタ
CCrystalObj::~CCrystalObj()
{
	// 設定されているマテリアルを解除してから、マテリアルを削除
	mpModel->SetMaterial(nullptr);
	SAFE_DELETE(mpMaterial);
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CCrystalObj::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpCollider)
	{
		// プレイヤーと衝突した場合
		if (other->Layer() == ELayer::ePlayer)
		{
			Kill();

			// プレイヤーの属性スロットに追加
			CElementManager::Instance()->AddElementEnergy(mElementType);
		}
	}
}

void CCrystalObj::Update()
{
}

void CCrystalObj::Render()
{
	// マテリアルを設定して、モデルを描画
	mpModel->SetMaterial(mpMaterial);
	mpModel->Render(Matrix());
}
