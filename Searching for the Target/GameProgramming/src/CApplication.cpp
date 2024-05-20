#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank2.obj", "res\\tank2.mtl"			//プレイヤー
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"		//的
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"		//弾

//#define MODEL_F14 "res\\f14.obj","res\\f14.mtl"
#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"

//#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"			//背景モデル

void CApplication::Start()
{
	//モデルファイルの入力
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	mModelBullet.Load(MODEL_BULLET);
	mBackGround.Load(MODEL_SKY);

	mEye = CVector(0.0f, 2.0f, 13.0f);

	CMatrix matrix;
	matrix.Print();

	mPlayer.SetModel(&mModel);
	mPlayer.SetScale(CVector(1.5f, 1.5f, 1.5f));
	mPlayer.SetPosition(CVector(0.0f, 0.0f, -5.0f));
	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));
	
	new CTarget(&mModelTarget, CVector(0.0f, 10.0f, -75.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	new CTarget(&mModelTarget, CVector(0.0f, 7.0f, -50.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	new CTarget(&mModelTarget, CVector(0.0f, 0.5f, -25.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

}

void CApplication::Update()
{
	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();
	//コリジョンマネージャの衝突判定
	CCollisionManager::GetInstance()->Collison();

	//弾が表示されない
	if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
	{
		CBullet *bullet = new CBullet();
		bullet->SetModel(&mModelBullet);
		bullet->SetScale(CVector(10.0f, 10.0f, 10.0f));
		bullet->SetPosition(CVector(0.0f, 0.0f, 10.0f) * mPlayer.GetMatrix());
		bullet->SetRotation(mPlayer.GetRotation());
		bullet->Update();
	}

	//カメラのパラメータを作成する
	CVector e, c, u;	//視点、注視点、上方向
	//視点を求める
	e = mPlayer.GetPosition() + CVector(0, 4, -10) * mPlayer.GetMatrixRotate();
	//注視点を求める
	c = mPlayer.GetPosition();
	//上方向を求める
	u = CVector(0, 1, 0) * mPlayer.GetMatrixRotate();
	//カメラ設定
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());

	mBackGround.Render();

	//タスクリストの削除
	CTaskManager::GetInstance()->Delete();
	//タスクマネージャの描画
	CTaskManager::GetInstance()->Render();
	//コリジョンマネージャの描画
	//CCollisionManager::GetInstance()->Render();

}