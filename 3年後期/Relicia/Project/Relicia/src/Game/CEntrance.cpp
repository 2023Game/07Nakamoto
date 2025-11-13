#include "CEntrance.h"
#include "CColliderMesh.h"

// コンストラクタ
CEntrance::CEntrance(const CVector& pos)
	: CObjectBase(ETag::eField, ETaskPriority::eBackground, 0, ETaskPauseType::eGame)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance");

	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, mpModel, false);

	Position(pos);

}

// デストラクタ
CEntrance::~CEntrance()
{
	SAFE_DELETE(mpColliderMesh);
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
