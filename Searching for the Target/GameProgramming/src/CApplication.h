#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CSwitch.h"
#include "CSlope.h"
#include "CMoveWall.h"
#include "CUi.h"
#include "CGame.h"

class CApplication
{
public:
	enum class EState
	{
		ESTART,		//ゲーム開始
		EPLAY,		//ゲーム中
		ECLEAR,		//ゲームクリア
		EOVER,		//ゲームオーバー
		ERSTART,	//再スタート
	};

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();
	//UIクラスのインスタンスを取得
	static CUi* GetUi();	

private:
	//ゲーム
	CGame* mpGame;

	//モデルクラスのインスタンス作成
	//プレイヤーのモデル
	CModel mModel;
	//的のモデル
	CModel mModelTarget;
	//床
	CModel mFloor;
	//オブジェクト
	CModel mObject;
	//背景モデル
	//CModel mBackGround;
	//坂のモデル
	CModel mModelSlope;
	//スイッチのモデル
	CModel mModelSwitch;
	
	CInput mInput;

	//カメラのパラメータを作成する
	CVector e, c, u;	//視点、注視点、上方向

	EState mState;

	//モデルビュー逆行列
	static CMatrix mModelViewInverse;

	static CUi* spUi;	//UIクラスのポインタ

	bool mFlag;
};