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
	mPlayers.clear();
}

// プレイヤーの追加
void CPlayerManager::AddPlayer(CPlayerBase* player)
{
	mPlayers.push_back(player);
}

// プレイヤーを取り除く
void CPlayerManager::RemovePlayer(CPlayerBase* player)
{
	auto find = std::find(mPlayers.begin(), mPlayers.end(), player);
	if (find == mPlayers.end()) return;
	mPlayers.erase(find);
}

// 操作キャラを切り替える
void CPlayerManager::ChangePlayer()
{
	int size = mPlayers.size();
	mCurrentIndex = (mCurrentIndex + 1) % size;

	// 全プレイヤーに、そのプレイヤーを操作するかどうかを設定
	for (int i = 0; i < size; i++)
	{
		mPlayers[i]->SetOperate(i == mCurrentIndex);
	}
}

// 更新
void CPlayerManager::Update()
{
	if (CInput::PushKey('C'))
	{
		ChangePlayer();
	}
}
