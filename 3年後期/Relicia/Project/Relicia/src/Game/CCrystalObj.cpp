#include "CCrystalObj.h"
#include "CModel.h"
#include "CColliderSphere.h"
#include "CElementManager.h"

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
	mpModel = CResourceManager::Load <CModel>("Crystal", mpCrystalData->modelPath);

	// 球コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eCrystal,
		10.0f, true
	);
	// プレイヤーと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer });
	
	Position(pos);
	Scale(3, 3, 3);
}

// デストラクタ
CCrystalObj::~CCrystalObj()
{
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
			CElementManager::Instance()->AddElement(mElementType);
		}
	}
}

void CCrystalObj::Update()
{
}

void CCrystalObj::Render()
{
	mpModel->Render(Matrix());
}
