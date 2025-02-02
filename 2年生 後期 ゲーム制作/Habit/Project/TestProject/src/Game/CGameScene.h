#pragma once
#include "CGameSceneBase.h"

//タイトルシーン
class CGameScene :public CGameSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
};
