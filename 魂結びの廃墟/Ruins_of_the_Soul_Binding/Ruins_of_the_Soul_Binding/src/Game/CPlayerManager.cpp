#include "CPlayerManager.h"
#include "CPlayerBase.h"
#include <assert.h>
#include "CInput.h"
#include "CIcon.h"
#include "CPlayer2.h"

#define RESERVED_CAPACITYE 5	// リストの初期容量

CPlayerManager* CPlayerManager::spInstance = nullptr;

// プレイヤー管理クラスのインスタンスを取得
CPlayerManager* CPlayerManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CPlayerManager::CPlayerManager()
	:CTask(ETaskPriority::ePlayer, 0, ETaskPauseType::eGame)
	, mCurrentIndex(0)
{
	assert(spInstance == nullptr);
	spInstance = this;

	// リストの初期容量の設定
	mPlayers.reserve(RESERVED_CAPACITYE);
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

	// アイコンの変更
	ETag tag = mPlayers[mCurrentIndex]->Tag();
	if (tag == ETag::eCat)
	{
		// 猫のアイコンを設定
		CIcon::Instance()->SetIcon((int)CIcon::EIcon::eCat);
	}
	else if (tag == ETag::ePlayer)
	{
		CPlayer2* player = dynamic_cast<CPlayer2*>(mPlayers[mCurrentIndex]);

		if (player->GatActingTogether())
		{
			// 猫のアイコンを設定
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::eTogether);
		}
		else
		{
			// 少女のアイコンを設定
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::ePlayer);
		}
	}
}

// プレイヤーのリストを取得
const std::vector<CPlayerBase*>& CPlayerManager::GetPlayers() const
{
	return mPlayers;
}

// 更新
void CPlayerManager::Update()
{
	if (CInput::PushKey('C'))
	{
		ChangePlayer();

		// アイコンの変更
		ETag tag = mPlayers[mCurrentIndex]->Tag();
		if (tag == ETag::eCat)
		{
			// 猫のアイコンを設定
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::eCat);
		}
		else if (tag == ETag::ePlayer)
		{
			// 少女のアイコンを設定
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::ePlayer);
		}
	}
}
