#include "CDoor.h"

// コンストラクタ
CDoor::CDoor(CObjectBase* owner, const CVector& pos, const CVector& angle, const CVector& size)
{
	// スイッチのモデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Wall");

	// スイッチのコライダーを取得
	CModel* colModel = CResourceManager::Get<CModel>("WallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// デストラクタ
CDoor::~CDoor()
{
	// コライダーを削除
	SAFE_DELETE(mpColliderMesh);
}

// 更新処理
void CDoor::Update()
{
}

// 描画処理
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
