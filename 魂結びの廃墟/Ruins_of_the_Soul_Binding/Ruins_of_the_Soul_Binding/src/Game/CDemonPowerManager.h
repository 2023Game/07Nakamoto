#pragma once
#include <vector>
#include "CTask.h"

class CDemonPower;

// 妖力の源の管理クラス
class CDemonPowerManager : public CTask
{
public:
	// 妖力の源のクラスのインスタンスを取得
	static CDemonPowerManager* Instance();

	// コンストラクタ
	CDemonPowerManager();
	// デストラクタ
	~CDemonPowerManager();

	// 妖力の源の追加
	void AddDemonPower(CDemonPower* power);
	// 妖力の源を取り除く
	void RemovePower(CDemonPower* power);

	// 妖力の源の数を取得
	int GetDemonPower();

private:
	// 妖力の源の管理クラスのインスタンスへのポインタ
	static CDemonPowerManager* spInstance;
	// 妖力の源のリスト
	std::vector<CDemonPower*> mDemonPowers;

};