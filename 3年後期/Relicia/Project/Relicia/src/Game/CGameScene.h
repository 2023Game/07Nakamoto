#pragma once
#include "CSceneBase.h"

class CField;
class CGameMenu;
class CSound;
class CEnemy;
class CNavManager;

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
	// 敵を生成
	void CreateEnemys();



	CField* mpField;
	CNavManager* mpNavManager;
	std::vector<CEnemy*> mpEnemys;

	CGameMenu* mpGameMenu;
};
