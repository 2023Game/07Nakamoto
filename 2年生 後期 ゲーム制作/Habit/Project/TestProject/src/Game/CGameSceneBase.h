#pragma once
#include "CSceneBase.h"

class CInventory;

// ゲームシーンのベースクラス
class CGameSceneBase : public CSceneBase
{
public:
	// コンストラクタ
	CGameSceneBase(EScene scene);
	// デストラクタ
	~CGameSceneBase();
	//シーン読み込み
	void Load();
	// 更新処理
	void Update();

private:
	CInventory* mpInventory;

};