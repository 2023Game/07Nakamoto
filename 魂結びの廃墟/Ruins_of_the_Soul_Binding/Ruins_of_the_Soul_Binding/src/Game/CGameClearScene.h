#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CGameClearUI;

// ゲームクリアシーン
class CGameClearScrene :public CSceneBase
{
public:
	// コンストラクタ
	CGameClearScrene();
	// デストラクタ
	~CGameClearScrene();
	//シーン読み込み
	void Load() override;
	//シーンの更新処理
	void Update() override;

private:
	CGameClearUI* mpGameClearUI;
};