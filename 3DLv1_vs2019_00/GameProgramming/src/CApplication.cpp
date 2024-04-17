#include "CApplication.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"

//クラスのstatic変数
CTexture CApplication::mTexture;

#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

CVector mEye;

//背景モデルデータの指定
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"
//モデルデータの指定
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"
//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{

	//カメラのパラメータを作成する
	CVector e, c, u; //視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラ設定
	//gluLookAt(視点X,視点Y,視点Z,中心X,中心Y,中心Z,上向X,上向Y,上向Z)
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMMING");

	//2Dの描画終了
	CCamera::End();
}