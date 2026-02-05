#include "CEntrance.h"
#include "CColliderMesh.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CField.h"

// コンストラクタ
CEntrance::CEntrance(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance"); 
	CModel* col = CResourceManager::Get<CModel>("Entrance_Col");
	CModel* ceilCol = CResourceManager::Get<CModel>("Entrance_Ceil_Col");

	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, col, false);
	mpColliderMeshCeil = new CColliderMesh(this, ELayer::eCeil, ceilCol, false);

	// コライダー表示をオンにする
	//mpColliderMesh->SetShow(true);
	//mpColliderMeshCeil->SetShow(true);

	Position(pos);

	CNavManager::Instance()->AddCollider(mpColliderMesh);
	CField::Instance()->AddObjectCollider(mpColliderMesh);
}

// デストラクタ
CEntrance::~CEntrance()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		navMgr->RemoveCollider(mpColliderMesh);
	}

	CField* field = CField::Instance();
	if (field != nullptr)
	{
		field->RemoveObjectCollider(mpColliderMesh);
	}

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
