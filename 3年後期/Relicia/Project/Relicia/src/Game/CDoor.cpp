#include "CDoor.h"

// コンストラクタ
CDoor::CDoor(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Door"),

	Position(pos);

}

// デストラクタ
CDoor::~CDoor()
{

}

// 更新
void CDoor::Update()
{
}

// 描画
void CDoor::Render()
{
	mpModel->Render(Matrix());
}
