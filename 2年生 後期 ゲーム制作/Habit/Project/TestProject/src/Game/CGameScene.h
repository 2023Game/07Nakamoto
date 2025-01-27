#pragma once
#include "CGameSceneBase.h"
class CGameMenu;
class CSound;
class CInventory;
class CHpUI;

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
	CInventory* mpInventory;
};
