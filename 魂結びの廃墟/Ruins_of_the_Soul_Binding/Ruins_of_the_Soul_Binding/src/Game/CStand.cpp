#include "CStand.h"
#include "CColliderMesh.h"
#include "CNavManager.h"

//コンストラクタ
CStand::CStand(const CVector& pos)
	: CObjectBase(ETag::eField)
	, mpStandWallColMesh(nullptr)
	, mpStandFloorColMesh(nullptr)
{
	mpStandModel = CResourceManager::Get <CModel>("Stand");

	Position(pos);

	// 土台の壁部分のコライダー作成
	CModel* standWallCol = CResourceManager::Get <CModel>("StandWallCol");
	mpStandWallColMesh = new CColliderMesh(this, ELayer::eWall, standWallCol, true);
	// 土台の床部分のコライダー作成
	CModel* standFloorCol = CResourceManager::Get <CModel>("StandFloorCol");
	mpStandFloorColMesh = new CColliderMesh(this, ELayer::eFloor, standFloorCol, true);

	// 経路探索用の遮蔽物チェックのコライダーに、扉のコライダーを登録
	CNavManager::Instance()->AddCollider(mpStandWallColMesh);
	CNavManager::Instance()->AddCollider(mpStandFloorColMesh);

}

// デストラクタ
CStand::~CStand()
{
	SAFE_DELETE(mpStandWallColMesh);
	SAFE_DELETE(mpStandFloorColMesh);
}

// 描画
void CStand::Render()
{
	mpStandModel->Render(Matrix());
}
