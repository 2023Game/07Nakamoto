#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CPlayerBase;

// プレイヤーの管理クラス
class CPlayerManager : public CTask
{
public:
	// プレイヤー管理クラスのインスタンスを取得
	static CPlayerManager* Instance();

	// コンストラクタ
	CPlayerManager();
	// デストラクタ
	~CPlayerManager();

	// プレイヤーの追加
	void AddPlayer(CPlayerBase* player);
	// プレイヤーを取り除く
	void RemovePlayer(CPlayerBase* player);

	// 操作キャラを切り替える
	void ChangePlayer();

	// 更新
	void Update();

private:
	// プレイヤー管理クラスのインスタンスへのポインタ
	static CPlayerManager* spInstance;

	// プレイヤーのリスト
	std::list<CPlayerBase*> mPlayers;

	int mCurrentIndex;	// 現在のインデックス
};