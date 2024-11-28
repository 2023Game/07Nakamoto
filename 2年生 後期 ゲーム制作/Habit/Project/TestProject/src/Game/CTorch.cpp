#include "CTorch.h"

// コンストラクタ
CTorch::CTorch(const CVector& pos, const CVector& angle, const CVector& size)
{
	// 壁のモデルデータ取得(変更予定)
	mpModel = CResourceManager::Get<CModel>("Wall");

	// 位置と向きとサイズを設定
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// デストラクタ
CTorch::~CTorch()
{
}

// 更新処理
void CTorch::Update()
{
}

// 描画処理
void CTorch::Render()
{
	mpModel->Render(Matrix());
}
