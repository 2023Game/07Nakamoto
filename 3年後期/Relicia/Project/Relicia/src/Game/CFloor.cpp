#include "CFloor.h"

// コンストラクタ
CFloor::CFloor(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Floor"),

	Position(pos);
}

// デストラクタ
CFloor::~CFloor()
{
}

// 更新
void CFloor::Update()
{
}

// 描画
void CFloor::Render()
{
	mpModel->Render(Matrix());
}
