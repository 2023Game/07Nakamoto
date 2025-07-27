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
#include "CItemObj.h"

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
	//mpTestWall = CResourceManager::Get<CModel>("TestWall");
	//mpTestWallCol = new CColliderMesh(this, ELayer::eWall, mpTestWall, true);

	// 経路探索用の遮蔽物チェックのコライダーに、フィールドの壁のコライダーを登録
	CNavManager::Instance()->AddCollider(mpWallColliderMesh);

	// 部屋の作成
	CreateRoomObjects();
	// ギミックの生成
	CreateGimmick();
	// アイテムを生成
	CreateItem();

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
	SAFE_DELETE(mpFloorColliderMesh);
	SAFE_DELETE(mpWallColliderMesh);
	// SAFE_DELETE(mpTestWallCol);
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
		new CNavNode(CVector(66.0f, 0.0f, 60.0f));
		new CNavNode(CVector(90.0f, 0.0f, 60.0f));
		new CNavNode(CVector(90.0f, 0.0f, 180.0f));
		new CNavNode(CVector(-90.0f, 0.0f, 180.0f));
		new CNavNode(CVector(200.0f, 0.0f, 60.0f));
		new CNavNode(CVector(200.0f, 0.0f, 180.0f));
		new CNavNode(CVector(120.0f, 0.0f, 180.0f));
		new CNavNode(CVector(120.0f, 0.0f, 450.0f));
		new CNavNode(CVector(-20.0f, 0.0f, 450.0f));
		new CNavNode(CVector(-20.0f, 0.0f, 395.0f));
		new CNavNode(CVector(-90.0f, 0.0f, 395.0f));
		new CNavNode(CVector(-90.0f, 0.0f, 310.0f));
		new CNavNode(CVector(-265.0f, 0.0f, 395.0f));
		new CNavNode(CVector(-265.0f, 0.0f, 256.5f));
		new CNavNode(CVector(-180.0f, 0.0f, 180.0f));
		new CNavNode(CVector(-180.0f, 0.0f, 60.0f));
		new CNavNode(CVector(-230.0f, 0.0f, 55.5f));
		new CNavNode(CVector(-275.0f, 0.0f, 55.5f));
		new CNavNode(CVector(-275.0f, 0.0f, -210.0f));
		new CNavNode(CVector(-155.5f, 0.0f, -210.0f));
		new CNavNode(CVector(-155.5f, 0.0f, -300.0f));
		new CNavNode(CVector(-155.5f, 0.0f, -59.0f));
		new CNavNode(CVector(-155.5f, 0.0f, -180.0f));
		new CNavNode(CVector(66.0f, 0.0f, -52.0f));
		new CNavNode(CVector(80.0f, 0.0f, -64.5f));
		new CNavNode(CVector(-20.0f, 0.0f, -300.0f));
		new CNavNode(CVector(-20.0f, 0.0f, -180.0f));
		new CNavNode(CVector(5.5f, 0.0f, -130.5f));
		new CNavNode(CVector(80.5f, 0.0f, -130.5f));
		new CNavNode(CVector(80.5f, 0.0f, -250.0f));

	}
}

// 部屋の作成
void CField::CreateRoomObjects()
{
	// 1-1教室を作生成
	CRoom* room1_1 = new CRoom
	(
		{
			{
				CVector(0.0f, ROOM_POS_Y, 0.0f),
				CVector(110.0f, ROOM_SIZE_Y, 87.45f)
			},
		},
		"1-1"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(33.0f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		room1_1
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-38.5f, 0.0f, 44.825f),
		CVector(0.0f, 0.0f, 0.0f),
		room1_1
	);

	// 1-2教室を作生成
	CRoom* room1_2 = new CRoom
	(
		{
			{
				CVector(-113.548f, ROOM_POS_Y, 0.0f),
				CVector(110.0f, ROOM_SIZE_Y, 87.45f),
			}
		},
		"1-2"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-150.7f, 0.0f, -44.825f),
		CVector(0.0f, 180.0f, 0.0f),
		room1_2
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-79.2f, 0.0f, -44.825f),
		CVector(0.0f, 180.0f, 0.0f),
		room1_2
	);

	// 美術室を生成
	CRoom* artRoom = new CRoom
	(
		{
			{
				CVector(143.275f, ROOM_POS_Y, -5.63751f),
				CVector(130.9f, ROOM_SIZE_Y, 95.425f),
			} 
		},
		"美術室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(99.0f, 0.0f, 43.175f),
		CVector(0.0f, 0.0f, 0.0f),
		artRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(187.0f, 0.0f, 43.175f),
		CVector(0.0f, 0.0f, 0.0f),
		artRoom
	);

	// 校長室を生成
	CRoom* principalRoom = new CRoom
	(
		{
			{
				CVector(141.471f, ROOM_POS_Y, 115.5f),
				CVector(75.83f, ROOM_SIZE_Y, 88.0f),
			}
		},
		"校長室"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(102.73f, 0.0f, 99.0f),
		CVector(0.0f, 90.0f, 0.0f),
		principalRoom
	);

	// 女子トイレ１を生成
	CRoom* womensRestroom1 = new CRoom
	(
		{
			{
				CVector(171.187f, ROOM_POS_Y, 245.987f),
				CVector(75.075f, ROOM_SIZE_Y, 34.375f),
			}
		},
		"女子トイレ1"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(132.82f, 0.0f, 258.5f),
		CVector(0.0f, 90.0f, 0.0f),
		womensRestroom1
	);

	// 男子トイレ１を生成
	CRoom* mensRestroom1 = new CRoom
	(
		{
			{
				CVector(171.187f, ROOM_POS_Y, 283.387f),
				CVector(75.075f, ROOM_SIZE_Y, 37.1249f),
			}
		},
		"男子トイレ1"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(132.82f, 0.0f, 280.5f),
		CVector(0.0f, 90.0f, 0.0f),
		mensRestroom1
	);

	// 空き教室を生成
	CRoom* emptyClassroom = new CRoom
	(
		{
			{
				CVector(171.187f, ROOM_POS_Y, 382.8f),
				CVector(75.075f, ROOM_SIZE_Y, 158.4f),
			}
		},
		"空き教室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(132.545f, 0.0f, 330.0f),
		CVector(0.0f, 90.0f, 0.0f),
		emptyClassroom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(132.545f, 0.0f, 434.5f),
		CVector(0.0f, 90.0f, 0.0f),
		emptyClassroom
	);

	// 2-1教室を生成
	CRoom* room2_1 = new CRoom
	(
		{
			{
				CVector(-32.9816f, ROOM_POS_Y, 247.225f),
				CVector(87.7248f, ROOM_SIZE_Y, 109.45f),
			}
		}, 
		"2-1"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.356f, 0.0f, 214.5f),
		CVector(0.0f, -90.0f, 0.0f),
		room2_1
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.356f, 0.0f, 280.5f),
		CVector(0.0f, -90.0f, 0.0f),
		room2_1
	);

	// 2-2教室を生成
	CRoom* room2_2 = new CRoom
	(
		{
			{
				CVector(56.3932f, ROOM_POS_Y, 247.225f),
				CVector(87.7248f, ROOM_SIZE_Y, 109.45f),
			}
		},
		"2-2"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-78.2194f, 0.0f, 280.225f),
		CVector(0.0f, 90.0f, 0.0f),
		room2_2
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-78.2194f, 0.0f, 214.225f),
		CVector(0.0f, 90.0f, 0.0f),
		room2_2
	);

	// 技術室を生成
	CRoom* technicalRoom = new CRoom
	(
		{
			{
				CVector(50.5403f, ROOM_POS_Y, 370.975f),
				CVector(99.4306f, ROOM_SIZE_Y, 134.75f),
			}
		},
		"技術室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(101.356f, 0.0f, 330.0f),
		CVector(0.0f, -90.0f, 0.0f),
		technicalRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-0.274993f, 0.0f, 330.0f),
		CVector(0.0f, 90.0f, 0.0f),
		technicalRoom
	);

	// 技術準備室を生成
	CRoom* technicalPreparationRoom = new CRoom
	(
		{
			{
				CVector(-90.5837f, ROOM_POS_Y, 434.913f),
				CVector(92.1827f, ROOM_SIZE_Y, 54.175f),
			}
		},
		"技術準備室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-43.392f, 0.0f, 429.0),
		CVector(0.0f, -90.0f, 0.0f),
		technicalPreparationRoom
	);

	// 被覆室を生成
	CRoom* coatingRoom = new CRoom
	(
		{
			{
				CVector(-206.25f, ROOM_POS_Y, 434.913f),
				CVector(135.85f, ROOM_SIZE_Y, 54.175f),
			}
		},
		"被覆室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-165.0f, 0.0f, 406.725f),
		CVector(0.0f, 180.0f, 0.0f),
		coatingRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-253.0f, 0.0f, 406.725f),
		CVector(0.0f, 180.0f, 0.0f),
		coatingRoom
	);

	// 被覆準備室を生成
	CRoom* coatingPreparationRoom = new CRoom
	(
		{
			{
				CVector(-303.188f, ROOM_POS_Y, 434.913f),
				CVector(54.7261f, ROOM_SIZE_Y, 54.175),
			}
		},
		"被覆準備室"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-275.0f, 0.0f, 444.4f),
		CVector(0.0f, -90.0f, 0.0f),
		coatingPreparationRoom
	);

	// 調理室を設定
	CRoom* kitchenRoom = new CRoom
	(
		{
			{
				CVector(-176.825f, ROOM_POS_Y, 337.838f),
				CVector(152.35f, ROOM_SIZE_Y, 81.675f),
			},

			{
				CVector(-213.125f, ROOM_POS_Y, 324.5f),
				CVector(79.75f, ROOM_SIZE_Y, 108.35f),
			},
		},
		"調理室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-121.0f, 0.0f, 379.775f),
		CVector(0.0f, 0.0f, 0.0f),
		kitchenRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-231.0f, 0.0f, 379.775f),
		CVector(0.0f, 0.0f, 0.0f),
		kitchenRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-231.0f, 0.0f, 269.225f),
		CVector(0.0f, 180.0f, 0.0f),
		kitchenRoom
	);

	// 調理準備室を生成
	CRoom* kitchenPreparationRoom = new CRoom
	(
		{
			{
				CVector(-136.125f, ROOM_POS_Y, 258.087f),
				CVector(70.95f, ROOM_SIZE_Y, 74.525f),
			}
		},
		"調理準備室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-121.0f, 0.0f, 296.45f),
		CVector(0.0f, 0.0f, 0.0f),
		kitchenPreparationRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-172.7f, 0.0f, 247.5f),
		CVector(0.0f, 90.0f, 0.0f),
		kitchenPreparationRoom
	);

	// 男子トイレ２を生成
	CRoom* mensRestroom2 = new CRoom
	(
		{
			{
				CVector(-134.475f, ROOM_POS_Y, 123.063f),
				CVector(70.95f, ROOM_SIZE_Y, 27.225f),
			}
		},
		"男子トイレ2"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-170.77f, 0.0f, 132.0f),
		CVector(0.0f, 90.0f, 0.0f),
		mensRestroom2
	);

	// 女子トイレ２を生成
	CRoom* womensRestroom2 = new CRoom
	(
		{
			{
				CVector(-134.475f, ROOM_POS_Y, 154.0f),
				CVector(70.95f, ROOM_SIZE_Y, 31.35f),
			}
		},
		"女子トイレ2"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-170.77f, 0.0f, 165.0f),
		CVector(0.0f, 90.0f, 0.0f),
		womensRestroom2
	);

	// 事務室を生成
	CRoom* officeRoom = new CRoom
	(
		{
			{
				CVector(-274.526f, ROOM_POS_Y, 207.233f),
				CVector(66.9476f, ROOM_SIZE_Y, 71.8158f),
			}
		},
		"事務室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-240.492f, 0.0f, 192.5f),
		CVector(0.0f, 90.0f, 0.0f),
		officeRoom
	);

	// 資料室を生成
	CRoom* documentRoom = new CRoom
	(
		{
			{
				CVector(-274.526f, ROOM_POS_Y, 140.25f),
				CVector(66.9476f, ROOM_SIZE_Y, 58.85f),
			}
		},
		"資料室"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-291.5f, 0.0f, 170.5f),
		CVector(0.0f, 0.0f, 0.0f),
		documentRoom
	);

	// 暗室を生成
	CRoom* darkRoom = new CRoom
	(
		{
			{
				CVector(-285.802f, ROOM_POS_Y, 88.0f),
				CVector(89.4987f, ROOM_SIZE_Y, 42.35f),
			}
		},
		"暗室"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-325.6f, 0.0f, 66.0f),
		CVector(0.0f, 180.0f, 0.0f),
		darkRoom
	);

	// 写真部の部屋を生成
	CRoom* photographyClubRoom = new CRoom
	(
		{
			{
				CVector(-308.688f, ROOM_POS_Y, 16.5f),
				CVector(43.7261f, ROOM_SIZE_Y, 97.35f),
			}
		},
		"写真部の部屋"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-285.725f, 0.0f, -16.5f),
		CVector(0.0f, -90.0f, 0.0f),
		photographyClubRoom
	);

	// 印刷室そ生成
	CRoom* printingRoom = new CRoom
	(
		{
			{
				CVector(-308.688f, ROOM_POS_Y, -71.5f),
				CVector(43.7261f, ROOM_SIZE_Y, 75.35f),
			}
		},
		"印刷室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-285.725f, 0.0f, -93.5f),
		CVector(0.0f, -90.0f, 0.0f),
		printingRoom
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-313.5f, 0.0f, -33.0f),
		CVector(0.0f, 0.0f, 0.0f),
		printingRoom
	);

	// PC教室を生成
	CRoom* pcRoom = new CRoom
	(
		{
			{
				CVector(-214.562f, ROOM_POS_Y, -77.0775f),
				CVector(89.2237f, ROOM_SIZE_Y, 241.605f),
			}
		},
		"PC教室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-260.274f, 0.0f, 21.725f),
		CVector(0.0f, 90.0f, 0.0f),
		pcRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-260.274f, 0.0f, -154.275f),
		CVector(0.0f, 90.0f, 0.0f),
		pcRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-168.85f, 0.0f, -154.275f),
		CVector(0.0f, -90.0f, 0.0f),
		pcRoom
	);

	//3-1教室の生成
	CRoom* room3_1 = new CRoom
	(
		{
			{
				CVector(-275.964f, ROOM_POS_Y, -269.362f),
				CVector(109.175f, ROOM_SIZE_Y, 87.725f),
			}
		},
		"3-1"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-308.541f, 0.0f, -224.4f),
		CVector(0.0f, 0.0f, 0.0f),
		room3_1
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-243.101f, 0.0f, -224.4f),
		CVector(0.0f, 0.0f, 0.0f),
		room3_1
	);

	// 男子トイレ3を生成
	CRoom* mensRestroom3 = new CRoom
	(
		{
			{
				CVector(-207.901f, ROOM_POS_Y, -269.362f),
				CVector(23.65f, ROOM_SIZE_Y, 87.725f),
			}
		},
		"男子トイレ3"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-198.0f, 0.0f, -224.675f),
		CVector(0.0f, 0.0f, 0.0f),
		mensRestroom3
	);

	// 女子トイレ3を生成
	CRoom* womensRestroom3 = new CRoom
	(
		{
			{
				CVector(-181.501f, ROOM_POS_Y, -269.362f),
				CVector(25.8499f, ROOM_SIZE_Y, 87.725f),
			}
		},
		"女子トイレ3"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-170.5f, 0.0f, -224.675f),
		CVector(0.0f, 0.0f, 0.0f),
		womensRestroom3
	);

	// 3-2教室を生成
	CRoom* room3_2 = new CRoom
	(
		{
			{
				CVector(-86.9f, ROOM_POS_Y, -243.393f),
				CVector(109.45f, ROOM_SIZE_Y, 87.725f),
			}
		},
		"3-2"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-120.175f, 0.0f, -198.43f),
		CVector(0.0f, 0.0f, 0.0f),
		room3_2
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-54.175f, 0.0f, -198.43f),
		CVector(0.0f, 0.0f, 0.0f),
		room3_2
	);

	// 視聴覚室を生成
	CRoom* audioVisualRoom = new CRoom
	(
		{
			{
				CVector(-86.9f, ROOM_POS_Y, -119.47f),
				CVector(109.45f, ROOM_SIZE_Y, 87.4502f),
			}
		},
		"視聴覚室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-55.0f, 0.0f, -164.295f),
		CVector(0.0f, 180.0f, 0.0f),
		audioVisualRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(-142.725f, 0.0f, -93.5f),
		CVector(0.0f, 90.0f, 0.0f),
		audioVisualRoom
	);

	// 視聴覚準備室を生成
	CRoom* audioVisualPreparationRoom = new CRoom
	(
		{
			{
				CVector(19.25f, ROOM_POS_Y, -95.2097f),
				CVector(99.55f, ROOM_SIZE_Y, 38.9305f),
			}
		},
		"視聴覚準備室"
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(-31.35f, 0.0f, -82.5f),
		CVector(0.0f, 90.0f, 0.0f),
		audioVisualPreparationRoom
	);

	// 物置の生成
	CRoom* storeRoom = new CRoom
	(
		{
			{
				CVector(43.0383f, ROOM_POS_Y, -192.25f),
				CVector(51.9734f, ROOM_SIZE_Y, 87.725f),
			}
		},
		"物置"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(49.5f, 0.0f, -147.287f),
		CVector(0.0f, 0.0f, 0.0f),
		storeRoom
	);

	// 職員室の生成
	CRoom* staffRoom = new CRoom
	(
		{
			{
				CVector(158.262f, ROOM_POS_Y, -210.04f),
				CVector(100.926f, ROOM_SIZE_Y, 154.43f),
			}
		},
		"職員室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(106.699f, 0.0f, -154.0f),
		CVector(0.0f, 90.0f, 0.0f),
		staffRoom
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(106.699f, 0.0f, -264.0f),
		CVector(0.0f, 90.0f, 0.0f),
		staffRoom
	);

	// 美術準備室を生成
	CRoom* artPreparationRoom = new CRoom
	(
		{
			{
				CVector(158.262f, ROOM_POS_Y, -93.0875f),
				CVector(100.926f, ROOM_SIZE_Y, 76.175f),
			}
		},
		"美術準備室"
	);
	// 左右のドアを生成
	new CDoorGimmickLR
	(
		CVector(106.699f, 0.0f, -77.0f),
		CVector(0.0f, 90.0f, 0.0f),
		artPreparationRoom
	);
	// 開き戸の生成
	new COpeningDoorGimmick
	(
		CVector(198.0f, 0.0f, -54.175f),
		CVector(0.0f, 0.0f, 0.0f),
		artPreparationRoom
	);

	CRoom* gymnasium = new CRoom
	(
		{
			{
				CVector(-30.3886f, ROOM_POS_Y, -358.05f),
				CVector(273.075f, ROOM_SIZE_Y, 86.3499f),
			}
		},
		"体育館"
	);
	CRoom* sportsWarehouse = new CRoom
	(
		{
			{
				CVector(-194.151f, ROOM_POS_Y, -358.05f),
				CVector(51.15f, ROOM_SIZE_Y, 86.3499f),
			}
		},

		"体育倉庫"
	);

}

// ギミックをの生成
void CField::CreateGimmick()
{
	// スイッチの生成
	CSwitch* switch1 = new CSwitch(CVector(50.0f, 0.0f, 230.0f));

	//CSwitchDoorGimmick* switchdoor1 = new CSwitchDoorGimmick
	//(
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, 0.0f, 0.0f),
	//	CVector(0.0f, -30.0f, 0.0f),
	//	"TestWallBrack", "TestWallBrack"
	//);
	//switchdoor1->AddSwitch(switch1);

}

// アイテムを生成
void CField::CreateItem()
{
	CItemObj* flameCharm = new CItemObj(ItemType::FlameCharm);
	flameCharm->Position(10.0f, 0.5f, 30.0f);
	flameCharm = new CItemObj(ItemType::FlameCharm);
	flameCharm->Position(20.0f, 0.5f, 30.0f);

	CItemObj* barrierCharm = new CItemObj(ItemType::BarrierCharm);
	barrierCharm->Position(-20.0f, 0.5f, 1.0f);

}

// レイとフィールドオブジェクトの衝突判定
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

// 更新
void CField::Update()
{
}

// 描画
void CField::Render()
{
	mpFloor->Render(Matrix());
	mpWall->Render(Matrix());
	//mpTestWall->Render(Matrix());
}
