#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMoveWall.h"
#include "CColliderHitManager.h"

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

	//カーソルの座標を設定する
	//SetCursorPos(400, 300);

	//モデルファイルの入力
	//mModel.Load(MODEL_TANK);
	CPlayer::GetModelPlayer()->Load(MODEL_TANK);
	//mModelTarget.Load(MODEL_TARGET);
	CTarget::GetModelTarget()->Load(MODEL_TARGET);
	//mFloor.Load(MODEL_FLOOR);
	CColliderMesh::GetModelFloor()->Load(MODEL_FLOOR);
	//mObject.Load(MODEL_OBJECT);
	CColliderMesh::GetModelObject()->Load(MODEL_OBJECT);
	mBackGround.Load(MODEL_SKY);
	//mModelSlope.Load(MODEL_SLOPE);
	CSlope::GetModelSlope()->Load(MODEL_SLOPE);
	//mModelSwitch.Load(MODEL_SPHERE);
	CSwitch::GetModelSwitch()->Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMoveFloor::GetModelFloor()->Load(MODEL_MOVEFLOOR);
	CMoveWall::GetModelWall()->Load(MODEL_MOVEWALL);

	mpGame = new CGame();

	/*
	//プレイヤー生成
	mPlayer.SetModel(&mModel);
	mPlayer.SetScale(CVector(1.5f, 1.5f, 1.5f));
	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));

	//的のコライダを生成
	new CTarget(&mModelTarget, CVector(-21.0f, 4.5f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(85.0f, 24.0f, 35.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(1.5f, 10.0f, 33.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(30.0f, 40.0f, -100.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(&mModelTarget, CVector(60.0f, 9.0f, 5.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	//スイッチの生成
	mSwhith.SetSwitch(&mModelSwitch,
		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//動く壁の生成
	mMoveWall.SetMoveWall(&mSwhith, 
		CVector(-19.0f, 5.0f, -1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//動く床
	mMoveFloor.SetMoveFloor(CVector(31.0f, 12.0f, -35.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//坂の生成
	mSlope.SetSlope(CVector(-16.5f, 5.325f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.065f, 0.8f), &mModelSlope);
	mSlope2.SetSlope(CVector(31.0f, 4.25f, -21.25f), CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 0.875f, 0.8f), &mModelSlope);

	//モデルから三角コライダを生成
	//親インスタンス
	// と親行列は無し
	//背景のモデル
	mColliderMesh4.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	//ステージのモデル
	mCSlope.ColliderMeshSet(&mSlope, mSlope.GetMatrix(), &mModelSlope,CCollider::ETag::ESLOPE);		//坂1
	mCSlope2.ColliderMeshSet(&mSlope2, mSlope2.GetMatrix(), &mModelSlope, CCollider::ETag::ESLOPE); //坂2
	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mFloor,CCollider::ETag::EFLOOR);				//床
	mColliderMesh3.ColliderMeshSet(nullptr, nullptr, &mObject);										//オブジェクト
	

	//ビルボードの生成
	//new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);

	//UIクラスの生成
	spUi = new CUi();

	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();
	*/
}

void CApplication::Update()
{
	switch (mState)
	{
	case CApplication::EState::ESTART:
		//スタート画面
		mpGame->Start();
		//Enterキーが押されたら
		if (mInput.Key(VK_RETURN))
		{
			//状態をプレイ中にする
			mState = EState::EPLAY;
		}
		break;



		//視点を求める
		e = CVector(60.0f, 2.0f, 10.0f);
		//注視点を求める
		c = CVector(0.0f,0.0f,0.0f);
		//上方向を求める
		u = CVector(0, 1, 0);
		//カメラ設定
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//モデルビュー行列の取得
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//逆行列の取得
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//背景
		mBackGround.Render();

		//マップの描画
		mFloor.Render();
		mObject.Render();

		//タスクマネージャの描画
		CTaskManager::GetInstance()->Render();

		spUi->State();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			mState = EState::EPLAY;
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



		//タスクマネージャの更新
		CTaskManager::GetInstance()->Update();
		//コリジョンマネージャの衝突判定
		CTaskManager::GetInstance()->Collision();

	//	mPlayer.HitCollision(mPlayer.GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

		CColliderHitManager::GetInstance()->Delete();

		//視点を求める
	//	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//注視点を求める
	//	c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//上方向を求める
	//	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
		//カメラ設定
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//モデルビュー行列の取得
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//逆行列の取得
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//背景
		mBackGround.Render();

		//マップの描画
		mFloor.Render();
		mObject.Render();

		//タスクリストの削除
		CTaskManager::GetInstance()->Delete();
		//タスクマネージャの描画
		CTaskManager::GetInstance()->Render();
		//コリジョンマネージャの描画
		CCollisionManager::GetInstance()->Render();

		spUi->Time();
		spUi->Render();

		if (CUi::GetPoint() == 5)
		{
			mState = EState::ECLEAR;		
		}
		break;
		
	case CApplication::EState::ECLEAR:
		//ゲームクリア画面
		mpGame->Clear();
		//Enterキーが押されたら
		if (mInput.Key(VK_RETURN))
		{
			//ゲームのインスタンス削除
			delete mpGame;
			//ゲームのインスタンス生成
			mpGame = new CGame();
			//状態をスタートにする
			mState = EState::ESTART;
		}
		break;



		//視点を求める
	//	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//注視点を求める
	//	c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//上方向を求める
	//	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
		//カメラ設定
		gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
		//モデルビュー行列の取得
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
		//逆行列の取得
		mModelViewInverse = mModelViewInverse.GetTranspose();
		mModelViewInverse.SetM(0, 3, 0);
		mModelViewInverse.SetM(1, 3, 0);
		mModelViewInverse.SetM(2, 3, 0);

		//背景
		mBackGround.Render();

		//タスクマネージャの描画
		CTaskManager::GetInstance()->Render();

		spUi->Clear();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			CTaskManager::GetInstance()->AllDelete();

			CUi::SetPoint();
			mState = EState::ERSTART;
		}

		break;

	case CApplication::EState::EOVER:
		//ゲームオーバー処理
		mpGame->Over();
		//Enterキーが押されたら
		if (mInput.Key(VK_RETURN))
		{
			//ゲームのインスタンス削除
			delete mpGame;
			//ゲームのインスタンス生成
			mpGame = new CGame();
			//状態をスタートにする
			mState = EState::ESTART;
		}
		break;

	case CApplication::EState::ERSTART:

		//プレイヤーの設定
	//	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	//	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));

		//的のコライダを生成
		new CTarget(&mModelTarget, CVector(-21.0f, 4.5f, -1.0f),
			CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(85.0f, 24.0f, 35.0f),
			CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(1.5f, 10.0f, 33.0f),
			CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(30.0f, 40.0f, -100.0f),
			CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
		new CTarget(&mModelTarget, CVector(60.0f, 9.0f, 5.0f),
			CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

		//スイッチの生成
	//	mSwhith.SetSwitch(&mModelSwitch,
	//		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
		//動く壁の生成
	//	mMoveWall.SetMoveWall(&mSwhith,
	//		CVector(-19.0f, 5.0f, -1.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

		//動く床
	//	mMoveFloor.SetMoveFloor(CVector(31.0f, 12.0f, -35.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
		
		mState = EState::EPLAY;

		break;
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

