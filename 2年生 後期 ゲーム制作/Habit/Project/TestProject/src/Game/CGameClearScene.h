#pragma once
#include "CSceneBase.h"

// ゲームクリアシーン
class CGameClearScrene :public CSceneBase
{
public:
	// コンストラクタ
	CGameClearScrene();
	// デストラクタ
	~CGameClearScrene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

};