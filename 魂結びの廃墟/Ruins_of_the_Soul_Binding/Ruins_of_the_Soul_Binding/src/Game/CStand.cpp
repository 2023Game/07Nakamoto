#include "CStand.h"
#include "CColliderMesh.h"

//コンストラクタ
CStand::CStand(const CVector& pos)
	: CObjectBase(ETag::eField)
{
	mpStandModel = CResourceManager::Get <CModel>("Stand");
	CModel* stand_col = CResourceManager::Get <CModel>("StandCol");

	Position(pos);
	mpStandColMesh = new CColliderMesh(this, ELayer::eField, stand_col, true);



}

// デストラクタ
CStand::~CStand()
{
	if (mpStandColMesh != nullptr)
	{
		SAFE_DELETE(mpStandColMesh);

	}
}

// 描画
void CStand::Render()
{
	mpStandModel->Render(Matrix());
}
