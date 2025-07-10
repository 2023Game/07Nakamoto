#pragma once
#include "CObjectBase.h"

class CDoor;
class CNavNode;
class CRoom;

// 左右ドアのギミック
class CDoorGimmickLR : public CObjectBase
{
public:
	// コンストラクタ
	CDoorGimmickLR(const CVector& pos, const CVector& angle, CRoom* room);
	// デストラクタ
	~CDoorGimmickLR();

	// オブジェクトの削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 左右のドアの開閉状態が切り替わった時に呼び出す処理
	void OnChangeDoor();

private:
	CDoor* mpDoorL;	// 左側のドア
	CDoor* mpDoorR;	// 右側のドア

	CNavNode* mpNavNodeL1;	// 左側のドアの経路探索ノード 1つ目
	CNavNode* mpNavNodeL2;	// 左側のドアの経路探索ノード 2つ目
	CNavNode* mpNavNodeR1;	// 右側のドアの経路探索ノード 1つ目
	CNavNode* mpNavNodeR2;	// 右側のドアの経路探索ノード 2つ目

};