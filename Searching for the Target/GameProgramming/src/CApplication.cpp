#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMoveWall.h"
#include "CColliderHitManager.h"
#include "CFloor.h"
#include "CObject.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//プレイヤー
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//的
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//弾

//#define MODEL_MAP "res\\map4.obj","res\\map4.mtl"	//試作マップ
//#define MODEL_MAP "res\\newmap3.obj","res\\newmap3.mtl"	//マップ

#define MODEL_FLOOR	 "res\\floor.obj","res\\floor.mtl"	//床
#define MODEL_OBJECT "res\\newobject.obj","res\\newobject.mtl"	//フィールドのオブジェクト

#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"		//背景仮

//#define MODEL_SLOPE "res\\slopeobj.obj","res\\slopeobj.mtl"		//坂(３つ)
#define MODEL_SLOPE "res\\slope.obj","res\\slope.mtl"		//坂

#define MODEL_SPHERE "res\\sphere.obj" ,"res\\sphere.mtl"			//球(スイッチ)

#define MODEL_MOVEWALL "res\\movecube.obj" ,"res\\movecube.mtl"			//動く壁

#define MODEL_MOVEFLOOR "res\\movefloor.obj","res\\movefloor.mtl"	//動く床

void CApplication::Start()
{
	mState = EState::ESTART;
	mFlag = false;

	//カーソルの座標を設定する
	//SetCursorPos(400, 300);

	//モデルファイルの入力
	CPlayer::GetModelPlayer()->Load(MODEL_TANK);
	CTarget::GetModelTarget()->Load(MODEL_TARGET);
	CFloor::GetModelFloor()->Load(MODEL_FLOOR);
	CObject::GetModelObject()->Load(MODEL_OBJECT);
	//mBackGround.Load(MODEL_SKY);
	CSlope::GetModelSlope()->Load(MODEL_SLOPE);
	CSwitch::GetModelSwitch()->Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMoveFloor::GetModelFloor()->Load(MODEL_MOVEFLOOR);
	CMoveWall::GetModelWall()->Load(MODEL_MOVEWALL);

	mpGame = new CGame();
}

void CApplication::Update()
{
	switch (mState)
	{
	case CApplication::EState::ESTART:
		//スタート画面
		mpGame->Start();
		
		if (mFlag == false)
		{
			//Enterキーが押されたら
			if (mInput.Key(VK_RETURN))
			{
				//状態をプレイ中にする
				mState = EState::EPLAY;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		break;

	case CApplication::EState::EPLAY:
		//ゲーム中
		mpGame->Update();
		//ゲームオーバーか判定
		if (mpGame->IsOver())
		{
			//状態をゲームオーバーにする
			mState = EState::EOVER;
		}
		//ゲームクリアか判定する
		if (mpGame->IsClear())
		{
			//状態をゲームクリアにする
			mState = EState::ECLEAR;
		}
		break;

	case CApplication::EState::ECLEAR:
		//ゲームクリア画面
		mpGame->Clear();

		if (mFlag == false)
		{
			//Enterキーが押されたら
			if (mInput.Key(VK_RETURN))
			{
				//ゲームのインスタンス削除
				delete mpGame;
				//ゲームのインスタンス生成
				mpGame = new CGame();
				//状態をスタートにする
				mState = EState::ESTART;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		break;

	case CApplication::EState::EOVER:
		//ゲームオーバー処理
		mpGame->Over();
		if (mFlag == false)
		{
			//Enterキーが押されたら
			if (mInput.Key(VK_RETURN))
			{
				//ゲームのインスタンス削除
				delete mpGame;
				//ゲームのインスタンス生成
				mpGame = new CGame();
				//状態をスタートにする
				mState = EState::ESTART;

				mFlag = true;
			}
		}

		if (!mInput.Key(VK_RETURN))
		{
			mFlag = false;
		}
		
		break;

	//case CApplication::EState::ERSTART:
	//	break;
	//default:
	//	break;

	}
}

CMatrix CApplication::mModelViewInverse;
//モデルビュー行列の取得
const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;
//UIクラスのインスタンスを取得
CUi* CApplication::GetUi()
{
	return spUi;	//インスタンスのポインタを返す
}

