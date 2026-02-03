#include "CSwitchFloor.h"
#include "CModel.h"

// コンストラクタ
CSwitchFloor::CSwitchFloor(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Switch_Floor");

	Position(pos);
}

// デストラクタ
CSwitchFloor::~CSwitchFloor()
{
}

// 更新処理
void CSwitchFloor::Update()
{
}

// 描画処理
void CSwitchFloor::Render()
{
	mpModel->Render(Matrix());
}
