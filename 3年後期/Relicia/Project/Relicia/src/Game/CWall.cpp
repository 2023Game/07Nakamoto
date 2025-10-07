#include "CWall.h"

// コンストラクタ
CWall::CWall(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Wall"),

	Position(pos);
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
