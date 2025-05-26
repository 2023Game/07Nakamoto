#include "CPlayerManager.h"
#include "CPlayerBase.h"
#include <assert.h>
#include "CInput.h"

CPlayerManager* CPlayerManager::spInstance = nullptr;

// プレイヤー管理クラスのインスタンスを取得
CPlayerManager* CPlayerManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CPlayerManager::CPlayerManager()
	:CTask(ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mCurrentIndex(0)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CPlayerManager::~CPlayerManager()
{
	spInstance = nullptr;

	auto itr = mPlayers.begin();
	auto end = mPlayers.end();
	while (itr != end)
	{
		CPlayerBase* del = *itr;
		itr = mPlayers.erase(itr);
		delete del;
	}
}

// プレイヤーの追加
void CPlayerManager::AddPlayer(CPlayerBase* player)
{
	mPlayers.push_back(player);
}

// プレイヤーを取り除く
void CPlayerManager::RemovePlayer(CPlayerBase* player)
{
	mPlayers.remove(player);
}

// 操作キャラを切り替える
void CPlayerManager::ChangePlayer()
{
	mCurrentIndex = (mCurrentIndex + 1) % mPlayers.size();
}

// 更新
void CPlayerManager::Update()
{
	mPlayers[mCurrentIndex]->Update();
}
