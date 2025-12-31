#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"

class CResultUI;

// リザルトシーン
class CResultScene : public CSceneBase
{
public:
	// コンストラクタ
	CResultScene();
	// デストラクタ
	~CResultScene();

	//シーン読み込み
	void Load() override;
	//シーンの更新処理
	void Update() override;

private:
	CResultUI* mpResultUI;
};