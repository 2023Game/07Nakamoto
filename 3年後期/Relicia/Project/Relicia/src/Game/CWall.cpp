#include "CWall.h"
#include "CNavManager.h"

// コンストラクタ
CWall::CWall(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Wall");

	Position(pos.X(),pos.Y(),pos.Z());
}

// デストラクタ
CWall::~CWall()
{
}

// 更新
void CWall::Update()
{
}

// 描画
void CWall::Render()
{
	mpModel->Render(Matrix());
}
