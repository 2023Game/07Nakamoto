#ifndef CUI_H
#define CUI_H

#include "CFont.h"

class CUi
{
public:
	CUi();

	//スタート画面
	void State();
	//クリア画面
	void Clear();

	//制限時間の設定
	void Time();
	//壊した的の数を加算
	void AddPoint();

	static int GetPoint();

	//描画
	void Render();

private:
	//時間
	int mTime;
	int mNum;

	static int mPoint;

	CFont mFont;
	CFont mFont2;
};

#endif // !CUI_H
