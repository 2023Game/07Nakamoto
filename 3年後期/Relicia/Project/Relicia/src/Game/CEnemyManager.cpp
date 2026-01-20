#include "CEnemyManager.h"
#include "CEnemy.h"

CEnemyManager* CEnemyManager::spInstance = nullptr;

// 敵の管理クラスのインスタンス
CEnemyManager* CEnemyManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CEnemyManager::CEnemyManager()
{
	spInstance = this;
}

// デストラクタ
CEnemyManager::~CEnemyManager()
{
	AllClear();
	spInstance = nullptr;
}

// 敵をリストに追加
void CEnemyManager::AddEnemy(CEnemy* enemy)
{
	mEnemys.push_back(enemy);
}

// 敵をリストから削除
void CEnemyManager::RemoveEnemy(CEnemy* enemy)
{
	auto result = std::remove(mEnemys.begin(), mEnemys.end(), enemy);
	mEnemys.erase(result, mEnemys.end());
}

// 敵のリストを削除
void CEnemyManager::AllClear()
{
	for (CEnemy* enemy : mEnemys)	enemy->Kill();
	mEnemys.clear();
}
