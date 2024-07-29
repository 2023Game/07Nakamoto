#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"
#include "CMovingDoor.h"

#include "CColliderHitManager.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//プレイヤー
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//的
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//弾

//#define MODEL_MAP "res\\map4.obj","res\\map4.mtl"	//試作マップ
//#define MODEL_MAP "res\\newmap3.obj","res\\newmap3.mtl"	//マップ

#define MODEL_FLOOR	 "res\\newfloor.obj","res\\newfloor.mtl"	//床
#define MODEL_OBJECT "res\\newobject.obj","res\\newobject.mtl"	//オブジェクト

#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"		//背景仮

//#define MODEL_SLOPE "res\\slopeobj.obj","res\\slopeobj.mtl"		//坂
#define MODEL_SLOPE "res\\newslope1.obj","res\\newslope1.mtl"		//坂

#define MODEL_SPHERE "res\\sphere.obj" ,"res\\sphere.mtl"		//球(スイッチ)

#define MODEL_BLUECUBE "res\\movecube.obj" ,"res\\movecube.mtl"	//青色の四角形

CApplication::CApplication()
	: mState(EState::EPLAY)
{

}

void CApplication::Start()
{
	//カーソルの座標を設定する
	//SetCursorPos(400, 300);

	//モデルファイルの入力
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	//mModelMap.Load(MODEL_MAP);

	mFloor.Load(MODEL_FLOOR);
	mObject.Load(MODEL_OBJECT);

	//mBackGround.Load(MODEL_SKY);
	mModelSlope.Load(MODEL_SLOPE);
	mModelSwitch.Load(MODEL_SPHERE);
	CBullet::GetModelBullet()->Load(MODEL_BULLET);
	CMovingDoor::GetModelCube()->Load(MODEL_BLUECUBE);

	mEye = CVector(0.0f, 2.0f, 13.0f);

	CMatrix matrix;
	matrix.Print();

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
	mSwhith3.SetSwitch(&mModelSwitch,
		CVector(-15.0f, 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
	//動く壁の生成
	mDoor.SetMovingDoor(&mSwhith3, 
		CVector(-18.5f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//坂の生成
	mSlope.SetSlope(CVector(-16.0f, 0.0f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f), &mModelSlope);

	//モデルから三角コライダを生成
	//親インスタンス
	// と親行列は無し
	//背景のモデル
	//mColliderMesh.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	//落ちてしまうところを補強するコライダ
	//mTriangle.SetColliderTriangle(nullptr, nullptr, CVector(-13.0f, -1.0f, 4.0f), CVector(13.0f, -1.0f, 4.0f), CVector(0.0f, -1.0f, -13.0f));

	//ステージのモデル
	mColliderMesh.ColliderMeshSet(&mSlope, mSlope.GetMatrix(), &mModelSlope,CCollider::ETag::ESLOPE);
	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mFloor,CCollider::ETag::EFLOOR);
	mColliderMesh3.ColliderMeshSet(nullptr, nullptr, &mObject);
	

	//ビルボードの生成
	//new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);

	//UIクラスの生成
	spUi = new CUi();

	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();

}

void CApplication::Update()
{
	switch (mState)
	{
	//スタート画面
	case CApplication::EState::ESTART:

		//視点を求める
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//注視点を求める
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//上方向を求める
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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

		mTriangle.Render();

		//タスクマネージャの描画
		CTaskManager::GetInstance()->Render();

		spUi->State();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			mState = EState::EPLAY;
		}

		break;

		//ゲーム中
	case CApplication::EState::EPLAY:
		//タスクマネージャの更新
		CTaskManager::GetInstance()->Update();
		//コリジョンマネージャの衝突判定
		CTaskManager::GetInstance()->Collision();

		mPlayer.HitCollision(mPlayer.GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

		CColliderHitManager::GetInstance()->Delete();

		//視点を求める
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//注視点を求める
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//上方向を求める
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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
		//mBackGround.Render();

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
		
	//クリア画面
	case CApplication::EState::ECLEAR:

		//視点を求める
		e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();

		//注視点を求める
		c = mPlayer.GetPosition() + CVector(0.0f, 3.0f, 0.0f);

		//c = mPlayer.GetPosition() + CVector(mPlayer.GetFx(),mPlayer.GetFy(),0.0f);

		//上方向を求める
		u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
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

		//試作マップの描画
		//mModelMap.Render();

		//タスクマネージャの描画
		CTaskManager::GetInstance()->Render();

		spUi->Clear();
		spUi->Render();

		if (mInput.Key(VK_RETURN))
		{
			CUi::SetPoint();
			mState = EState::EPLAY;
		}

		break;

	//case CApplication::EState::EOVER:
	//	break;
	case CApplication::EState::ERSTART:

		

		break;
	//default:
	//	break;

	}
	

}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::GetUi()
{
	return spUi;	//インスタンスのポインタを返す
}

