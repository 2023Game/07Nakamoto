#include "CEntrance.h"
#include "CColliderMesh.h"

// コンストラクタ
CEntrance::CEntrance(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance"); 
	CModel* col = CResourceManager::Get<CModel>("Entrance_Col");
	CModel* archCol = CResourceManager::Get<CModel>("Entrance_Arch_Col");

	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, col, false);
	mpColliderMeshArch = new CColliderMesh(this, ELayer::eFloor, archCol, false);

	// コライダー表示をオンにする
	//mpColliderMesh->SetShow(true);

	Position(pos);

}

// デストラクタ
CEntrance::~CEntrance()
{
	SAFE_DELETE(mpColliderMesh);
	SAFE_DELETE(mpColliderMeshArch);
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
