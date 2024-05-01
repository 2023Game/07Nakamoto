#pragma once
#include "CModel.h"
#include "CInput.h"

class CApplication
{
public:
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

private:
	//モデルクラスのインスタンス作成
	CModel mModel;
	//背景モデル
	CModel mBackGround;

	CInput mInput;
	CVector mEye;
};