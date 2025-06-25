#include "CSwitchDoorGimmick.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CSwitchDoor.h"
#include "CSwitch.h"

// ドアに配置するノードの距離
#define DOOR_NODE_DIST 10.0f

// コンストラクタ
CSwitchDoorGimmick::CSwitchDoorGimmick(const CVector& pos, const CVector& angle, const CVector& openPos,
	std::string modelName, std::string colName)
	: CObjectBase(ETag::eGimmick)
	, mpSwitchDoor(nullptr)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
{
	// 位置や回転値を反映
	Position(pos);
	Rotate(angle);

	mpSwitchDoor = new CSwitchDoor(pos, angle, openPos, modelName, colName);
	mpSwitchDoor->SetOwner(this);
	mpSwitchDoor->SetOnChangeFunc(std::bind(&CSwitchDoorGimmick::OnChangeDoor, this));

	// 経路探索用のノードを作成
	mpNavNode1 = new CNavNode(pos + VectorZ() * DOOR_NODE_DIST);
	mpNavNode2 = new CNavNode(pos - VectorZ() * DOOR_NODE_DIST);

	// 各方向のドアのノード同士を強制的に繋ぐ
	mpNavNode1->AddForcedConnectNode(mpNavNode2);

	// 現時点でのドアの開閉状態を経路探索ノードに反映
	OnChangeDoor();
}

// デストラクタ
CSwitchDoorGimmick::~CSwitchDoorGimmick()
{
	if (mpSwitchDoor != nullptr)
	{
		mpSwitchDoor->SetOwner(nullptr);
		mpSwitchDoor->Kill();
	}

	// 経路探索ノードを全て削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode1);
		SAFE_DELETE(mpNavNode2);
	}
}

// 接続するスイッチを追加
void CSwitchDoorGimmick::AddSwitch(CSwitch* sw)
{
	mpSwitchDoor->AddSwitch(sw);
}

//経路探索ノード１つ目が開閉状態が切り替わった時に呼び出す処理
void CSwitchDoorGimmick::OnChangeDoor()
{
	// 開いているかどうかを求める
	bool opened = mpSwitchDoor == nullptr || mpSwitchDoor->IsOpened();

	// 開き戸の経路が繋がっているか設定
	mpNavNode1->SetEnableConnect(mpNavNode2, opened);
	mpNavNode2->SetEnableConnect(mpNavNode1, opened);
}