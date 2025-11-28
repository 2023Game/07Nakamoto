#include "CCrystalObj.h"
#include "CColliderSphere.h"

// コンストラクタ
CCrystalObj::CCrystalObj(ElementType type, CVector pos)
	: CInteractObject(ETaskPriority::eCrystal, 0, ETaskPauseType::eGame)
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
	mpModel = CResourceManager::Get <CModel>(mpCrystalData->modelPath);

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
}

// デストラクタ
CCrystalObj::~CCrystalObj()
{
	// 読み込んだモデルデータ
	SAFE_DELETE(mpModel);
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}
