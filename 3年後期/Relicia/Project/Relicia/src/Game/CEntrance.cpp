#include "CEntrance.h"

// コンストラクタ
CEntrance::CEntrance(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Wall_Entrance");

	Position(pos);

}

// デストラクタ
CEntrance::~CEntrance()
{
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
