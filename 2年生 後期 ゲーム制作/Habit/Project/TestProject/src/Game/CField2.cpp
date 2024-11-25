#include "CField2.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CWall2.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"

CField2* CField2::spInstance = nullptr;

CField2* CField2::Instance()
{
	return spInstance;
}

CField2::CField2()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	assert(spInstance == nullptr);
	spInstance = this;
	//床の生成
	mpModel = CResourceManager::Get<CModel>("Map_mini_floor");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	// 壁を生成
	CreateWalls();
	CreateFieldObjects();
	// 経路探索用のノードを作成
	CreateNavNodes();
}

CField2::~CField2()
{
	spInstance = nullptr;

	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CField2::Update()
{
}

// レイとフィールドオブジェクトの衝突判定
bool CField2::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	// フィールドのオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	//// 壁との衝突判定
	//for (CWall2* wall : mWalls)
	//{
	//	if (wall->CollisionRay(start, end, &tHit))
	//	{
	//		// まだほかに衝突していない場合か、
	//		// 既に衝突しているコライダーより近い場合は、
	//		if (!isHit || tHit.dist < hit->dist)
	//		{
	//			// 衝突情報を更新
	//			*hit = tHit;
	//			isHit = true;
	//		}
	//	}
	//}

	return isHit;

}

// 壁を生成
void CField2::CreateWalls()
{
	mpMap = CResourceManager::Get<CModel>("Map_mini");

	// マップ①
	new CMoveFloor
	(
		mpMap,
		CVector(0.0f, 1.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
}

// オブジェクトの生成
void CField2::CreateFieldObjects()
{
}

void CField2::CreateNavNodes()
{
	
}

void CField2::Render()
{
	mpModel->Render(Matrix());
}
