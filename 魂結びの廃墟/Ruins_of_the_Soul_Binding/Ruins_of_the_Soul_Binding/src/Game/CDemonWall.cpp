#include "CDemonWall.h"
#include "CColliderMesh.h"
#include "CNavManager.h"

// コンストラクタ
CDemonWall::CDemonWall()
	: CObjectBase(ETag::eField)
	, mpModel(nullptr)
	, mpColliderMesh(nullptr)
{
	mpModel = CResourceManager::Get<CModel>("DemonWall");

	CModel* col = CResourceManager::Get<CModel>("DemonWallCol");
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, col, true);

	// 経路探索用の遮蔽物チェックのコライダーに、妖力の壁のコライダーを登録
	CNavManager::Instance()->AddCollider(mpColliderMesh);
}

// デストラクタ
CDemonWall::~CDemonWall()
{
	SAFE_DELETE(mpColliderMesh);
}

// 描画
void CDemonWall::Render()
{
	mpModel->Render(Matrix());
}
