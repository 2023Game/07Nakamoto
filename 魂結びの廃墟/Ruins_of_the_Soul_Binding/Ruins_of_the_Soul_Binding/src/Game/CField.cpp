#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CRDoor.h"
#include "CLDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CSceneManager.h"
#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "CSwitch.h"

CField* CField::spInstance = nullptr;

CField* CField::Instance()
{
	return spInstance;
}

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	spInstance = this;

	mpFloor = CResourceManager::Get<CModel>("Floor");
	mpWall = CResourceManager::Get<CModel>("Wall");
	mpWallCol = CResourceManager::Get<CModel>("WallCol");

	mpFloorColliderMesh = new CColliderMesh(this, ELayer::eField, mpFloor, true);
	mpWallColliderMesh = new CColliderMesh(this, ELayer::eWall, mpWallCol, true);

	// 経路探索用の遮蔽物チェックのコライダーに、フィールドの壁のコライダーを登録
	CNavManager::Instance()->AddCollider(mpWallColliderMesh);

	// 右のドアの生成
	mpRDoor = new CRDoor
	(
		CVector(37.95f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(28.05f, 0.0f, 44.825f)
	);

	// 左のドアの生成
	mpLDoor = new CLDoor
	(
		CVector(28.05f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(37.95f, 0.0f, 44.825f)
	);

	// 右のドアの生成
	mpRDoor = new CRDoor
	(
		CVector(-33.55f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-43.45, 0.0f, 44.825f)
	);

	// 左のドアの生成
	mpLDoor = new CLDoor
	(
		CVector(-43.45, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(-33.55f, 0.0f, 44.825f)
	);


	new CDemonPowerManager();

	// 妖力の源
	new CDemonPower(CVector(30.0f, 5.0f, 10.0f));
	// 妖力の源
	new CDemonPower(CVector(20.0f, 5.0f, 10.0f));
	// 妖力の源
	new CDemonPower(CVector(10.0f, 5.0f, 10.0f));

	// スイッチの生成
	new CSwitch(CVector(30.0f, 0.0f, 30.0f));

	// 経路探索用のノードを作成
	CreateNavNodes();

	//CreateFieldObjects();
}

CField::~CField()
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
CColliderMesh* CField::GetFloorCol() const
{
	return mpFloorColliderMesh;
}

// 壁のコライダーの取得
CColliderMesh* CField::GetWallCol() const
{
	return mpWallColliderMesh;
}

// 経路探索用のノードを作成
void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();

	if (navMgr != nullptr)
	{
		// ノードの設定
		new CNavNode(CVector(-40.0f, 0.0f, 60.0f));
		new CNavNode(CVector(-40.0f, 0.0f, 30.0f));
		new CNavNode(CVector(35.0f, 0.0f, 60.0f));
		new CNavNode(CVector(35.0f, 0.0f, 30.0f));
	}
}

void CField::CreateFieldObjects()
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

	// 動かない床�@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動く床�@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床�A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 回転する床�@
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// 動かない床�A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// 動かない床�A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// 動かない床�B（坂道）
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

bool CField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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

void CField::Update()
{
}

void CField::Render()
{
	mpFloor->Render(Matrix());
	mpWall->Render(Matrix());
}
