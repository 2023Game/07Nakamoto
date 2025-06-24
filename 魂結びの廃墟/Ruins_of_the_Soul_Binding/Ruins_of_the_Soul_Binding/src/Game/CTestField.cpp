#include "CTestField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CRDoor.h"
#include "CLDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CSceneManager.h"

CTestField* CTestField::spInstance = nullptr;

CTestField* CTestField::Instance()
{
	return spInstance;
}

CTestField::CTestField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	spInstance = this;

	mpFloor = CResourceManager::Get<CModel>("Field");
	mpWall = CResourceManager::Get<CModel>("Wall");
	mpWallCol = CResourceManager::Get<CModel>("WallCol");

	mpFloorColliderMesh = new CColliderMesh(this, ELayer::eFloor, mpFloor, true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, mpWallCol, true);

	// 経路探索用の遮蔽物チェックのコライダーに、フィールドの壁のコライダーを登録
	CNavManager::Instance()->AddCollider(mpWallColliderMesh);

	new CLDoor
	(
		CVector(30.0f, 0.0f, 44.175f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(39.9f, 0.0f, 44.175f)
	);
	new CRDoor
	(
		CVector(39.9f, 0.0f, 44.175f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(30.0f, 0.0f, 44.175f)
	);

	new CLDoor
	(
		CVector(-36.0f, 0.0f, 44.175f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-26.1, 0.0f, 44.175f)
	);
	new CRDoor
	(
		CVector(-26.1, 0.0f, 44.175f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-36.0f, 0.0f, 44.175f)
	);

	new CLDoor
	(
		CVector(37.8027f, 0.0f, 132.73f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(27.903f, 0.0f, 132.73f)
	);
	new CRDoor
	(
		CVector(27.903f, 0.0f, 132.73f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(37.8027f, 0.0f, 132.73f)
	);

	new CLDoor
	(
		CVector(-28.1973f, 0.0f, 132.73f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-38.0973f, 0.0f, 132.73f)
	);
	new CRDoor
	(
		CVector(-38.0973f, 0.0f, 132.73f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-28.1973f, 0.0f, 132.73f)
	);

	// 経路探索用のノードを作成
	CreateNavNodes();
}

CTestField::~CTestField()
{
	if (mpFloorColliderMesh != nullptr)
	{
		delete mpFloorColliderMesh;
		mpFloorColliderMesh = nullptr;
	}
	if (mpWallColliderMesh != nullptr)
	{
		delete mpWallColliderMesh;
		mpWallColliderMesh = nullptr;
	}
}

// 床のコライダーを取得
CColliderMesh* CTestField::GetFloorCol() const
{
	return mpFloorColliderMesh;
}

// 壁のコライダーの取得
CColliderMesh* CTestField::GetWallCol() const
{
	return mpWallColliderMesh;
}

// 経路探索用のノードを作成
void CTestField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();

	if (navMgr != nullptr)
	{
		// ノードの設定
		new CNavNode(CVector(-31.5f, 0.0f, 60.0f));
		new CNavNode(CVector(-31.5f, 0.0f, 28.0f));
		new CNavNode(CVector(35.0f, 0.0f, 28.0f));
		new CNavNode(CVector(35.0f, 0.0f, 60.0f));

		new CNavNode(CVector(32.0f, 0.0f, 120.0f));
		new CNavNode(CVector(32.0f, 0.0f, 145.0f));
		new CNavNode(CVector(-34.0f, 0.0f, 145.0f));
		new CNavNode(CVector(-34.0f, 0.0f, 120.0f));
	}
}

void CTestField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");

	new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(50.0f, 0.0f, 0.0f), 10.0f
	);
	new CRotateFloor
	(
		mpCylinderModel,
		CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
		1.0f
	);

	// 動かない床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動く床①
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 回転する床①
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床②
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// 動かない床③（坂道）
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// 電撃エフェクト
	CLineEffect* le = new CLineEffect(ETag::eField);
	le->SetTexture("LightningBolt");
	le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	le->SetAnimData(&mEffectAnimData);

	CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	int div = 3;
	float width = 5.0f;
	le->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		le->AddPoint(pos, width, width);
	}
}

bool CTestField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// 衝突情報保存用
	CHitInfo tHit;
	// 衝突したかどうかのフラグ
	bool isHit = false;

	// 床のオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpFloorColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}
	// 壁のオブジェクトとの衝突判定
	if (CCollider::CollisionRay(mpWallColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = true;
	}

	return isHit;
}

void CTestField::Update()
{
}

void CTestField::Render()
{
	mpFloor->Render(Matrix());
	mpWall->Render(Matrix());
}
