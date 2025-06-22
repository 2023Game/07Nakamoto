#include "CDoorGimmickLR.h"
#include "CDoor.h"
#include "CLDoor.h"
#include "CRDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"

// 左右のドアの中心位置からのオフセット量
#define DOOR_OFFSET_DIST 4.95f
// ドアに配置するノードの距離
#define DOOR_NODE_DIST 10.0f

// コンストラクタ
CDoorGimmickLR::CDoorGimmickLR(const CVector& pos, const CVector& angle)
	: CObjectBase(ETag::eGimmick)
	, mpDoorL(nullptr)
	, mpDoorR(nullptr)
	, mpNavNodeL1(nullptr)
	, mpNavNodeL2(nullptr)
	, mpNavNodeR1(nullptr)
	, mpNavNodeR2(nullptr)
{
	// 位置や回転値を反映
	Position(pos);
	Rotation(angle);

	// 左右のドアが閉まっているときの座標を求める計算
	CVector posL = pos - VectorX() * DOOR_OFFSET_DIST;
	CVector posR = pos + VectorX() * DOOR_OFFSET_DIST;

	// 左側のドアを作成
	mpDoorL = new CLDoor(posL, angle, posR);
	mpDoorL->SetOwner(this);
	mpDoorL->SetOnChangeFunc(std::bind(&CDoorGimmickLR::OnChangeDoor, this));

	// 右側のドアを作成
	mpDoorR = new CRDoor(posR, angle, posL);
	mpDoorR->SetOwner(this);
	mpDoorR->SetOnChangeFunc(std::bind(&CDoorGimmickLR::OnChangeDoor, this));

	// 経路探索用のノードを作成
	mpNavNodeL1 = new CNavNode(posL + VectorZ() * DOOR_NODE_DIST);
	mpNavNodeL2 = new CNavNode(posL - VectorZ() * DOOR_NODE_DIST);
	mpNavNodeR1 = new CNavNode(posR + VectorZ() * DOOR_NODE_DIST);
	mpNavNodeR2 = new CNavNode(posR - VectorZ() * DOOR_NODE_DIST);

	// 各方向のドアのノード同士を強制的に繋ぐ
	mpNavNodeL1->AddForcedConnectNode(mpNavNodeL2);
	mpNavNodeR1->AddForcedConnectNode(mpNavNodeR2);

	// 繋がるノードを自動検索する際に、各ノードの繋がないノードを設定
	mpNavNodeL1->AddBlockedNode(mpNavNodeR1);
	mpNavNodeL1->AddBlockedNode(mpNavNodeR2);
	mpNavNodeL2->AddBlockedNode(mpNavNodeR1);
	mpNavNodeL2->AddBlockedNode(mpNavNodeR2);

	// 現時点でのドアの開閉状態を経路探索ノードに反映
	OnChangeDoor();

}

// デストラクタ
CDoorGimmickLR::~CDoorGimmickLR()
{
	// 左側のドアが残っていたら、持ち主を解除してドアを削除
	if (mpDoorL != nullptr)
	{
		mpDoorL->SetOwner(nullptr);
		mpDoorL->Kill();
	}

	// 右側のドアが残っていたら、持ち主を解除してドアを削除
	if (mpDoorR != nullptr)
	{
		mpDoorR->SetOwner(nullptr);
		mpDoorR->Kill();
	}

	// 経路探索ノードを全て削除
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNodeL1);
		SAFE_DELETE(mpNavNodeL2);
		SAFE_DELETE(mpNavNodeR1);
		SAFE_DELETE(mpNavNodeR2);
	}
}

// オブジェクトの削除を伝える関数
void CDoorGimmickLR::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが左側のドアだったら
	if (obj == mpDoorL)
	{
		mpDoorL = nullptr;
		// ドアがなくなったので、左側の経路をオンにする
		mpNavNodeL1->SetEnableConnect(mpNavNodeL2, true);
		mpNavNodeL2->SetEnableConnect(mpNavNodeL1, true);
	}

	// 削除されたオブジェクトが左側のドアだったら
	if (obj == mpDoorR)
	{
		mpDoorR = nullptr;
		// ドアがなくなったので、右側の経路をオンにする
		mpNavNodeR1->SetEnableConnect(mpNavNodeR2, true);
		mpNavNodeR2->SetEnableConnect(mpNavNodeR1, true);
	}

}

// 左右のドアの開閉状態が切り替わった時に呼び出す処理
void CDoorGimmickLR::OnChangeDoor()
{
	// 左右のドアが開いているかどうかを求める
	bool openedL = mpDoorL == nullptr || mpDoorL->IsOpened();
	bool openedR = mpDoorR == nullptr || mpDoorR->IsOpened();

	// 左側のドアの経路が繋がっているか設定
	//（左ドアが空いている & 右ドアが閉まっている）
	mpNavNodeL1->SetEnableConnect(mpNavNodeL2, openedL && !openedR);
	mpNavNodeL2->SetEnableConnect(mpNavNodeL1, openedL && !openedR);

	// 右側のドアの経路が繋がっているか設定
	//（右ドアが空いている & 左ドアが閉まっている）
	mpNavNodeR1->SetEnableConnect(mpNavNodeR2, openedR && !openedL);
	mpNavNodeR2->SetEnableConnect(mpNavNodeR1, openedR && !openedL);

}
