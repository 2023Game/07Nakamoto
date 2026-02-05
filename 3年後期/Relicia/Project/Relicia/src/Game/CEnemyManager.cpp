#include "CEnemyManager.h"
#include <assert.h>
#include "CCactus.h"
#include "CMashroom.h"
#include "Maths.h"
#include "CBspMap.h"
#include "CField.h"

#define INITIAL_ENEMY_SPAWN 1	// 初期の敵のスポーン数

CEnemyManager* CEnemyManager::spInstance = nullptr;

// 敵の管理クラスのインスタンス
CEnemyManager* CEnemyManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CEnemyManager();
	}
	return spInstance;
}

// コンストラクタ
CEnemyManager::CEnemyManager()
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CEnemyManager::~CEnemyManager()
{
	AllClear();

	if (spInstance == this)
	{
		spInstance = nullptr;
	}
}

// インスタンスを削除
void CEnemyManager::ClearInstance()
{
	if (spInstance != nullptr)
	{
		spInstance->Kill();
		spInstance = nullptr;
	}
}

// 敵をリストに追加
void CEnemyManager::AddEnemy(CEnemy* enemy)
{
	mpEnemys.push_back(enemy);
}

// 敵をリストから削除
void CEnemyManager::RemoveEnemy(CEnemy* enemy)
{
	auto it = std::find(mpEnemys.begin(), mpEnemys.end(), enemy);
	mpEnemys.erase(it, mpEnemys.end());

	(*it)->Kill();
}

// 敵のリストを削除
void CEnemyManager::AllClear()
{
	for (CEnemy* enemy : mpEnemys)
	{
		if (enemy || !enemy->IsKill())
		{
			enemy->Kill();
		}
	}
	mpEnemys.clear();
}

// 敵を生成
void CEnemyManager::CreateEnemys()
{
	CField* field = CField::Instance();

	for (int i = 0; i < INITIAL_ENEMY_SPAWN; i++)
	{
		// サボテンの敵を作成
		CCactus* cactus = new CCactus();
		cactus->Position(field->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Enemy));
		cactus->InitNav();
		CEnemyManager::Instance()->AddEnemy(cactus);

		// キノコの敵を作成
		CMashroom* mashroom = new CMashroom();
		mashroom->Position(field->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Enemy));
		mashroom->InitNav();
		CEnemyManager::Instance()->AddEnemy(mashroom);
	}
}

// 敵をランダムに選んでスポーンさせる
void CEnemyManager::RandomEnemySpawn()
{
	CField* field = CField::Instance();
	CEnemy* enemy = nullptr;

	int num = Math::Rand(0, 1);

	switch (num)
	{
	case 0:
		// サボテンの敵を作成
		enemy = new CCactus();
		break;
	case 1:
		// キノコの敵を作成
		enemy = new CMashroom();
		break;
	}

	if (!enemy) return;

	enemy->Position(field->GetMapData()->GetRoomRandomFloorPos(CBspMap::EOccupyType::Enemy));
	enemy->InitNav();
	AddEnemy(enemy);
}
