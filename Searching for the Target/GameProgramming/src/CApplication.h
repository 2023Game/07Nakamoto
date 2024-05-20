#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"

class CApplication
{
public:
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

private:
	CPlayer mPlayer;	
	
	//モデルクラスのインスタンス作成
	CModel mModel;
	//弾のモデル
	CModel mModelBullet;
	//的のモデル
	CModel mModelTarget;
	//背景モデル
	CModel mBackGround;

	CInput mInput;
	CVector mEye;
};