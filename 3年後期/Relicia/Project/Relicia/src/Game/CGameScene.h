#pragma once
#include "CSceneBase.h"

class CField;
class CGameMenu;
class CSound;
class CEnemy;
class CNavManager;
class CObjectBase;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();

	//シーンの更新処理
	void Update() override;

private:

	CField* mpField;

	CGameMenu* mpGameMenu;
};
