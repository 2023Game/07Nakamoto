#include "CPillar.h"

// コンストラクタ
CPillar::CPillar(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Pillar"),

	Position(pos);

}

// デストラクタ
CPillar::~CPillar()
{
}

// 更新
void CPillar::Update()
{
}

// 描画
void CPillar::Render()
{
	mpModel->Render(Matrix());
}
