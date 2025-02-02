#pragma once
#include "CGameSceneBase.h"

//タイトルシーン
class CGameScene3 :public CGameSceneBase
{
public:
	//コンストラクタ
	CGameScene3();
	//デストラクタ
	~CGameScene3();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
};
