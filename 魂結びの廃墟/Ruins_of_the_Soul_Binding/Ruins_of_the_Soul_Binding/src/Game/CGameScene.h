#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CField;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load() override;
	//シーンの更新処理
	void Update() override;

private:
	CGameMenu* mpGameMenu;
	CField* mpField;
};
