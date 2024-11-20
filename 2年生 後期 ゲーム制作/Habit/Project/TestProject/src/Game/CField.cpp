#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"


CField* CField::spInstance = nullptr;

CField* CField::Instance()
{
	return spInstance;
}

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	assert(spInstance == nullptr);
	spInstance = this;

	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	// 壁を生成
	CreateWalls();
	CreateFieldObjects();
	// 経路探索用のノードを作成
	CreateNavNodes();
}

CField::~CField()
{
	spInstance = nullptr;

	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// 壁を生成
void CField::CreateWalls()
{
	// 壁①生成
	CWall* wall = new CWall
	(
		CVector(20.0f,  1.0f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(5.0f,  5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// 生成した壁を壁のリストに追加

	// 壁②生成
	wall = new CWall
	(
		CVector(-50.0f, 1.0f, -50.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// 生成した壁を壁のリストに追加

	// 壁③生成
	wall = new CWall
	(
		CVector(250.0f, 1.0f, 200.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// 生成した壁を壁のリストに追加
}

void CField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");

	mpMap1 = CResourceManager::Get<CModel>("Map1");

	// マップ①
	/*new CMoveFloor
	(
		mpMap1,
		CVector(0.0f, 1.0f, 0.0f), CVector(10.0f, 10.0f, 10.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);*/

	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(50.0f, 0.0f, 0.0f), 10.0f
	//);
	//new CRotateFloor
	//(
	//	mpCylinderModel,
	//	CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
	//	1.0f
	//);

	//// 動かない床①
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動く床①
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(20.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動かない床②
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 回転する床①
	//new CRotateFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
	//	0.5f
	//);
	//// 動かない床②
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// 動かない床②
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 50.0f, 0.0f), 5.0f
	//);
	//// 動かない床③（坂道）
	//CMoveFloor* mf = new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//mf->Rotate(0.0f, 0.0f, 30.0f);

	//// 電撃エフェクト
	//CLineEffect* le = new CLineEffect(ETag::eField);
	//le->SetTexture("LightningBolt");
	//le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	//le->SetAnimData(&mEffectAnimData);

	//CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	//CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	//int div = 3;
	//float width = 5.0f;
	//le->AddPoint(startPos, width, width);
	//for (int i = 0; i < div; i++)
	//{
	//	float alpha = (float)(i + 1) / div;
	//	CVector pos = CVector::Lerp(startPos, endPos, alpha);
	//	le->AddPoint(pos, width, width);
	//}
}

void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 壁①の周りの経路探索ノード
		new CNavNode(CVector( 30.0f, 0.0f,  35.0f));
		new CNavNode(CVector( 10.0f, 0.0f,  35.0f));
		new CNavNode(CVector( 10.0f, 0.0f, -35.0f));
		new CNavNode(CVector( 30.0f, 0.0f, -35.0f));

		// 壁②の周りの経路探索ノード
		new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
		new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

		// 壁③の周りの経路探索ノード
		new CNavNode(CVector(285.0f, 1.0f, 190.0f));
		new CNavNode(CVector(285.0f, 0.0f, 210.0f));
		new CNavNode(CVector(215.0f, 0.0f, 210.0f));
		new CNavNode(CVector(215.0f, 0.0f, 190.0f));
		
	}
}

// レイとフィールドオブジェクトの衝突判定
bool CField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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

	// 壁との衝突判定
	for (CWall* wall : mWalls)
	{
		if (wall->CollisionRay(start, end, &tHit))
		{
			// まだほかに衝突していない場合か、
			// 既に衝突しているコライダーより近い場合は、
			if (!isHit || tHit.dist < hit->dist)
			{
				// 衝突情報を更新
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;
}


void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}
