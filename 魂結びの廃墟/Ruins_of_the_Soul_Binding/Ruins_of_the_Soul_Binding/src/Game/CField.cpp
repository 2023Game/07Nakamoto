#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CDoorGimmickLR.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CSceneManager.h"
#include "CDemonPower.h"
#include "CDemonPowerManager.h"
#include "COpeningDoorGimmick.h"
#include "CSwitch.h"
#include "CSwitchDoorGimmick.h"
#include "CRoom.h"

#define ROOM_SIZE_Y 33.0f
#define ROOM_POS_Y	ROOM_SIZE_Y * 0.5f

CField* CField::spInstance = nullptr;

// インスタンスのポインタを取得
CField* CField::Instance()
{
	return spInstance;
}

// コンストラクタ
CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	spInstance = this;

	mpFloor = CResourceManager::Get<CModel>("Floor");
	mpWall = CResourceManager::Get<CModel>("Wall");
	mpWallCol = CResourceManager::Get<CModel>("WallCol");

	mpFloorColliderMesh = new CColliderMesh(this, ELayer::eFloor, mpFloor, true);
	mpWallColliderMesh = new CColliderMesh
	(
		this, ELayer::eWall, mpWallCol,
		true, 1.0f,
		10, 1, 10
	);

	// テスト用
	mpTestWall = CResourceManager::Get<CModel>("TestWall");
	mpTestWallCol = new CColliderMesh(this, ELayer::eWall, mpTestWall, true);

	// 経路探索用の遮蔽物チェックのコライダーに、フィールドの壁のコライダーを登録
	CNavManager::Instance()->AddCollider(mpWallColliderMesh);

	CRoom* room = new CRoom
	(
		CVector(0.0f, ROOM_POS_Y, 0.0f),
		CVector(110.0f, ROOM_SIZE_Y, 87.45f),
		"1-1"
	);
	room = new CRoom
	(
		CVector(143.275f, ROOM_POS_Y, -5.63751f),
		CVector(130.9f, ROOM_SIZE_Y, 95.425f),
		"1-2"
	);
	room = new CRoom
	(
		CVector(141.471f, ROOM_POS_Y, 115.5f),
		CVector(75.83f, ROOM_SIZE_Y, 88.0f),
		"校長室"
	);


	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(33.0f,0.0f,44.825f),
		CVector(0.0f,0.0f,0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-38.5f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(99.0f, 0.0f, 43.175f),
		CVector(0.0f, 0.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(187.0f, 0.0f, 43.175f),
		CVector(0.0f, 0.0f, 0.0f)
	);

	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.355f, 0.0f, 214.5f),
		CVector(0.0f, -90.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.355f, 0.0f, 280.5f),
		CVector(0.0f, -90.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.355f, 0.0f, 330.0f),
		CVector(0.0f, -90.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-43.392f, 0.0f, 429.0),
		CVector(0.0f, -90.0f, 0.0f)
	);

	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(132.545f, 0.0f, 330.0f),
		CVector(0.0f, 90.0f, 0.0f)
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(132.545f, 0.0f, 434.5f),
		CVector(0.0f, 90.0f, 0.0f)
	);


	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(102.73f,0.0f,99.0f),
		CVector(0.0f,90.0f,0.0f)
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(132.82f, 0.0f, 258.5f),
		CVector(0.0f, 90.0f, 0.0f)
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(132.82f, 0.0f, 280.5f),
		CVector(0.0f, 90.0f, 0.0f)
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-170.77f, 0.0f, 132.0f),
		CVector(0.0f, 90.0f, 0.0f)
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-170.77f, 0.0f, 165.0f),
		CVector(0.0f, 90.0f, 0.0f)
	);


	// スイッチの生成
	CSwitch* switch1 = new CSwitch(CVector(50.0f, 0.0f, 230.0f));

	CSwitchDoorGimmick* switchdoor1 = new CSwitchDoorGimmick
	(
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, -30.0f, 0.0f),
		"TestWallBrack", "TestWallBrack"
	);
	switchdoor1->AddSwitch(switch1);

	// 妖力の源のマネージャーを生成
	new CDemonPowerManager();

	// テスト用妖力の源
	new CDemonPower(CVector(20.0f, 5.0f, 20.0f));

	//// 妖力の源
	//new CDemonPower(CVector(140.0f, 5.0f, 120.0f));
	//// 妖力の源
	//new CDemonPower(CVector(140.0f, 5.0f, -20.0f));
	//// 妖力の源
	//new CDemonPower(CVector(180.0f, 5.0f, 380.0f));
	//// 妖力の源
	//new CDemonPower(CVector(-30.0f, 5.0f, 220.0f));
	//// 妖力の源
	//new CDemonPower(CVector(-200.0f, 5.0f, 120.0f));

	// 経路探索用のノードを作成
	CreateNavNodes();

	//CreateFieldObjects();
}

// デストラクタ
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

	SAFE_DELETE(mpTestWallCol);
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
		new CNavNode(CVector(-90.0f, 0.0f, 60.0f));
		new CNavNode(CVector(65.0f, 0.0f, 60.0f));
		new CNavNode(CVector(90.0f, 0.0f, 60.0f));
		new CNavNode(CVector(90.0f, 0.0f, 180.0f));
		new CNavNode(CVector(-90.0f, 0.0f, 180.0f));
		new CNavNode(CVector(200.0f, 0.0f, 60.0f));
		new CNavNode(CVector(200.0f, 0.0f, 180.0f));
		new CNavNode(CVector(120.0f, 0.0f, 180.0f));
		new CNavNode(CVector(120.0f, 0.0f, 450.0f));
		new CNavNode(CVector(-20.0f, 0.0f, 450.0f));
		new CNavNode(CVector(-20.0f, 0.0f, 400.0f));
		new CNavNode(CVector(-90.0f, 0.0f, 310.0f));
		new CNavNode(CVector(-180.0f, 0.0f, 180.0f));
		new CNavNode(CVector(-180.0f, 0.0f, 60.0f));

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
	mpTestWall->Render(Matrix());
}
