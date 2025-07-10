#include "COpeningDoorGimmick.h"
#include "COpeningDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CRoom.h"

// 開き戸の中心位置からのオフセット量
#define DOOR_OFFSET_DIST 4.95f
// 開き戸に配置するノードの距離
#define DOOR_NODE_DIST 14.0f

// コンストラクタ
COpeningDoorGimmick::COpeningDoorGimmick(const CVector& pos, const CVector& angle, CRoom* room)
	: CObjectBase(ETag::eGimmick)
	, mpOpeningDoor(nullptr)
	, mpNavNode1(nullptr)
	, mpNavNode2(nullptr)
{
	// 位置や回転値を反映
	Position(pos);
	Rotation(angle);

	// 開いた時の回転を求める計算
	CVector rot = angle + CVector(0.0f, 90.0f, 0.0f);

	// 開き扉を作成
	mpOpeningDoor = new COpeningDoor(pos, angle, rot, room);
	mpOpeningDoor->SetOwner(this);
	mpOpeningDoor->SetOnChangeFunc(std::bind(&COpeningDoorGimmick::OnChangeDoor, this));
	mpOpeningDoor->SetRoom(room);

	// 経路探索用のノードを作成
	mpNavNode1 = new CNavNode(pos + VectorZ() * DOOR_NODE_DIST - CVector(0.0f, 0.0f, DOOR_OFFSET_DIST));
	mpNavNode2 = new CNavNode(pos - VectorZ() * DOOR_NODE_DIST - CVector(0.0f, 0.0f, DOOR_OFFSET_DIST));

	// 各方向のドアのノード同士を強制的に繋ぐ
	mpNavNode1->AddForcedConnectNode(mpNavNode2);

	// 現時点でのドアの開閉状態を経路探索ノードに反映
	OnChangeDoor();

}

	// デストラクタ
COpeningDoorGimmick::~COpeningDoorGimmick()
{
	if (mpOpeningDoor != nullptr)
	{
		mpOpeningDoor->SetOwner(nullptr);
		mpOpeningDoor->Kill();
	}

	// 経路探索ノードを全て削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode1);
		SAFE_DELETE(mpNavNode2);
	}
}

// オブジェクトの削除を伝える関数
void COpeningDoorGimmick::DeleteObject(CObjectBase* obj)
{
	// 削除された場合
	if (obj == mpOpeningDoor)
	{
		mpOpeningDoor = nullptr;
		// 開き戸がなくなったので、経路をオンにする
		mpNavNode1->SetEnableConnect(mpNavNode2, true);
		mpNavNode2->SetEnableConnect(mpNavNode1, true);
	}
}

//開き戸の経路探索ノード１つ目が開閉状態が切り替わった時に呼び出す処理
void COpeningDoorGimmick::OnChangeDoor()
{
	// 開いているかどうかを求める
	bool opened = mpOpeningDoor == nullptr || mpOpeningDoor->IsOpened();

	// 開き戸の経路が繋がっているか設定
	mpNavNode1->SetEnableConnect(mpNavNode2, opened);
	mpNavNode2->SetEnableConnect(mpNavNode1, opened);
}
