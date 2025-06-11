#include "CDemonPowerManager.h"
#include "CDemonPower.h"
#include <assert.h>
#include "CBoss.h"

#define RESERVED_CAPACITYE 7	// リストの初期容量

// 妖力の管理クラスのインスタンス
CDemonPowerManager* CDemonPowerManager::spInstance = nullptr;

// 妖力の源のクラスのインスタンスを取得
CDemonPowerManager* CDemonPowerManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CDemonPowerManager::CDemonPowerManager()
	: CTask(ETaskPriority::eEnemy, 0, ETaskPauseType::eGame)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// リストの初期容量の設定
	mDemonPowers.reserve(RESERVED_CAPACITYE);
}

// デストラクタ
CDemonPowerManager::~CDemonPowerManager()
{
	spInstance = nullptr;
	mDemonPowers.clear();
}

// 妖力の源の追加
void CDemonPowerManager::AddDemonPower(CDemonPower* power)
{
	mDemonPowers.push_back(power);
}

// 妖力の源を取り除く
void CDemonPowerManager::RemovePower(CDemonPower* power)
{
	auto find = std::find(mDemonPowers.begin(), mDemonPowers.end(), power);
	if (find == mDemonPowers.end()) return;
	mDemonPowers.erase(find);
}

// 妖力の源の数を取得
int CDemonPowerManager::GetDemonPower()
{
	return mDemonPowers.size();
}