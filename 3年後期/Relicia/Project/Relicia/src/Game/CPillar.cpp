#include "CPillar.h"

// コンストラクタ
CPillar::CPillar(const CVector& pos)
{
	mpModel = CResourceManager::Get<CModel>("Pillar");
	CModel* col = CResourceManager::Get<CModel>("Pillar_Col");

	mpColliderMesh = new CColliderMesh(this, ELayer::eFloor, col, false);

	// コライダー表示をオンにする
	mpColliderMesh->SetShow(true);

	Position(pos);

}

// デストラクタ
CPillar::~CPillar()
{
	SAFE_DELETE(mpColliderMesh);
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
