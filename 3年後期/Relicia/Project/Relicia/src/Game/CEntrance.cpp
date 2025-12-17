#include "CEntrance.h"
#include "CColliderMesh.h"
#include "CNavNode.h"

// コンストラクタ
CEntrance::CEntrance(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance"); 
	CModel* col = CResourceManager::Get<CModel>("Entrance_Col");
	CModel* ceilCol = CResourceManager::Get<CModel>("Entrance_Ceil_Col");

	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, col, false);
	mpColliderMeshCeil = new CColliderMesh(this, ELayer::eCeil, ceilCol, false);

	new CNavNode(CVector(pos.X(),pos.Y(),pos.Z() + 10.0f));
	// コライダー表示をオンにする
	//mpColliderMesh->SetShow(true);
	//mpColliderMeshCeil->SetShow(true);

	Position(pos);
}

// デストラクタ
CEntrance::~CEntrance()
{
	SAFE_DELETE(mpColliderMesh);
	SAFE_DELETE(mpColliderMeshCeil);
}

// 更新
void CEntrance::Update()
{
}

// 描画
void CEntrance::Render()
{
	mpModel->Render(Matrix());
}
