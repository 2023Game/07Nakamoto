#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CSwitch.h"
#include "CSlope.h"
#include "CMovingDoor.h"
#include "CUi.h"

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

	CApplication();

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

	static CUi* GetUi();	//UIクラスのインスタンスを取得

private:

	//プレイヤー
	CPlayer mPlayer;
	//坂
	CSlope mSlope;
	//スイッチ
	//CSwitch mSwitch;
	//CSwitch mSwitch2;

	CSwitch mSwhith3;
	CMovingDoor mDoor;

	//モデルクラスのインスタンス作成
	CModel mModel;
	//的のモデル
	CModel mModelTarget;
	//マップのモデル
	CModel mModelMap;
	//背景モデル
	CModel mBackGround;
	//坂のモデル
	CModel mModelSlope;
	//スイッチのモデル
	CModel mModelSwitch;

	//青い四角形のモデル
	CModel mModelCube;

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;
	CColliderMesh mColliderMesh2;
	//補強コライダ
	CColliderTriangle mTriangle;
	
	CInput mInput;

	//カメラのパラメータを作成する
	CVector e, c, u;	//視点、注視点、上方向
	CVector mEye;

	CCollider mC;

	EState mState;

	//モデルビュー逆行列
	static CMatrix mModelViewInverse;

	static CUi* spUi;		//UIクラスのポインタ
};