#ifndef CUI_H
#define CUI_H

#include "CFont.h"

class CUi
{
public:
	//デフォルトコンストラクタ
	CUi();

	//デストラクタ
	~CUi();

	//スタート画面
	void State();
	//クリア画面
	void Clear();

	//制限時間の管理
	void Time();
	//制限時間の取得
	static int GetTime();

	//描画
	void Render();

private:
	//時間
	static int mTime;
	//60フレーム数える
	int mNum;

	CFont mFont;
};

#endif // !CUI_H
