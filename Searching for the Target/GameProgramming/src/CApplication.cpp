#include "CApplication.h"
#include "CVector.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CTarget.h"
#include "CBillBoard.h"

//OpenGL
#include "glut.h"

#define MODEL_TANK "res\\tank.obj", "res\\tank.mtl"			//プレイヤー
#define MODEL_TARGET "res\\target.obj", "res\\target.mtl"	//的
#define MODEL_BULLET "res\\bullet.obj", "res\\bullet.mtl"	//弾

#define MODEL_MAP "res\\map3.obj","res\\map3.mtl"			//試作マップ
#define MODEL_SKY "res\\sky.obj","res\\sky.mtl"				//背景仮

#define MODEL_REDCUBE "res\\RedCube.obj" ,"res\\RedCube.mtl"	//赤色の四角形
#define MODEL_BLUECUBE "res\\BlueCube.obj" ,"res\\BlueCube.mtl"	//青色の四角形

//#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"	//背景モデル

void CApplication::Start()
{
	//カーソルの座標を設定する
	//SetCursorPos(400, 300);

	//モデルファイルの入力
	mModel.Load(MODEL_TANK);
	mModelTarget.Load(MODEL_TARGET);
	mModelMap.Load(MODEL_MAP);
	mBackGround.Load(MODEL_SKY);

	mRed.Load(MODEL_REDCUBE);

	CBullet::GetModelBullet()->Load(MODEL_BULLET);

	CMoveFloor::GetModelRedCube()->Load(MODEL_REDCUBE);
	CMoveFloor::GetModelBlueCube()->Load(MODEL_BLUECUBE);

	mEye = CVector(0.0f, 2.0f, 13.0f);

	CMatrix matrix;
	matrix.Print();

	//プレイヤー生成
	mPlayer.SetModel(&mModel);
	mPlayer.SetScale(CVector(1.5f, 1.5f, 1.5f));
	mPlayer.SetPosition(CVector(0.0f, 2.0f, -5.0f));
	mPlayer.SetRotation(CVector(0.0f, 180.0f, 0.0f));
	
	//的のコライダを生成
	new CTarget(&mModelTarget, CVector(-20.0f, 5.0f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);

	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -50.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	new CTarget(&mModelTarget, CVector(0.0f, 2.75f, -25.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	//背景モデルから三角コライダを生成
	//親インスタンスと親行列は無し
	mColliderMesh.ColliderMeshSet(nullptr, nullptr, &mBackGround);

	mColliderMesh2.ColliderMeshSet(nullptr, nullptr, &mModelMap);

	mColliderMeshRed.ColliderMeshSet(nullptr, nullptr, &mRed);

	//ビルボードの生成
	new CBillBoard(CVector(-0.6f, 3.0f, -10.0f), 1.0f, 1.0f);
}

void CApplication::Update()
{
	//タスクマネージャの更新
	CTaskManager::GetInstance()->Update();
	//コリジョンマネージャの衝突判定
	// 削除 CCollisionManager::GetInstance()->Collison();
	CTaskManager::GetInstance()->Collision();

	//カメラのパラメータを作成する
	CVector e, c, u;	//視点、注視点、上方向
	//視点を求める
	e = mPlayer.GetPosition() + CVector(0, 5, -8) * mPlayer.GetMatrixRotate();
	
	// 確認用の視点 右
	//e = mPlayer.GetPosition() + CVector(-10.0f, 1.0f, 0.0f) * mPlayer.GetMatrixRotate();

	//e = mPlayer.GetPosition() + CVector(-5.0f, 1.0f, 3.0f) * mPlayer.GetMatrixRotate();

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

	mBackGround.Render();

	//試作マップの描画
	mModelMap.Render(); 

	mRed.Render();

	//タスクリストの削除
	CTaskManager::GetInstance()->Delete();
	//タスクマネージャの描画
	CTaskManager::GetInstance()->Render();
	//コリジョンマネージャの描画
	CCollisionManager::GetInstance()->Render();

}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}