#pragma once
#include "CObjectBase.h"

class CSwitchDoor;
class CSwitch;
class CNavNode;

class CSwitchDoorGimmick : public CObjectBase
{
public:
	// コンストラクタ
	CSwitchDoorGimmick(const CVector& pos, const CVector& angle, const CVector& openPos,
		std::string modelName, std::string colName);
	// デストラクタ
	~CSwitchDoorGimmick();

	// 接続するスイッチを追加
	void AddSwitch(CSwitch* sw);

	//経路探索ノード１つ目が開閉状態が切り替わった時に呼び出す処理
	void OnChangeDoor();

private:
	CSwitchDoor* mpSwitchDoor;	// スイッチが押されたら開く扉

	CNavNode* mpNavNode1;	//扉の経路探索ノード１つ目
	CNavNode* mpNavNode2;	//扉の経路探索ノード２つ目
};