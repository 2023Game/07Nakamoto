#pragma once
#include "CObjectBase.h"

class COpeningDoor;
class CNavNode;

// 開き戸のギミック
class COpeningDoorGimmick : public CObjectBase
{
public:
	// コンストラクタ
	COpeningDoorGimmick(const CVector& pos, const CVector& angle);
	// デストラクタ
	~COpeningDoorGimmick();

	// オブジェクトの削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	//開き戸の経路探索ノード１つ目が開閉状態が切り替わった時に呼び出す処理
	void OnChangeDoor();

private:
	COpeningDoor* mpOpeningDoor;	// 開き戸

	CNavNode* mpNavNode1;	//開き戸の経路探索ノード１つ目
	CNavNode* mpNavNode2;	//開き戸の経路探索ノード２つ目
};