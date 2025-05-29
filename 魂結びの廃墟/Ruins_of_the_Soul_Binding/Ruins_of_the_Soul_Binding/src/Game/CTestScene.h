#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CField;

//タイトルシーン
class CTestScene :public CSceneBase
{
public:
	//コンストラクタ
	CTestScene();
	//デストラクタ
	~CTestScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
	CField* mpField;
};
