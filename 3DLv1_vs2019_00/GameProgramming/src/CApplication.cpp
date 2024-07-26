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
//敵モデル(Knight)
#define MODEL_KNIGHT "res\\knight\\knight_low.x"

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
	//カメラの設定
	mActionCamera.Set(5.0f, -15.0f, 180.0f);

	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);

	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);
	
	//mKnight.Load("res\\knight\\knight_low.x");
	mKnight.Load(MODEL_KNIGHT);
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 1:移動
	mKnight.SeparateAbunatuibSet(0, 1530, 1830, "idle1");	// 2:待機
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 3:ダミー
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 4:ダミー
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 5:ダミー
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 6:ダミー
	mKnight.SeparateAbunatuibSet(0, 440, 520, "attack1");	// 7:Attack1
	mKnight.SeparateAbunatuibSet(0, 520, 615, "attack2");	// 8:Attack2
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		// 9:ダミー
	mKnight.SeparateAbunatuibSet(0, 10, 80, "walk");		//10:ダミー
	mKnight.SeparateAbunatuibSet(0, 1160, 1260, "walk");	//11:ダウン

	//敵の初期化設定
	mXEnemy.Init(&mKnight);

	//敵の配置
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));

	mXEnemy.ChangeAnimation(2, true, 200);

	mpPaladin = new CPaladin();
	mpPaladin->Position(CVector(-1.0f, 0.0f, 5.0f));
	mpPaladin->ChangeAnimation(1, true, 60);

	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{
	mpPaladin->Update();
	//キャラクタークラスの更新
	mXPlayer.Update();
	//敵の更新
	mXEnemy.Update();

	//衝突処理
	CCollisionManager::Instance()->Collision();

	//カメラ設定
	mActionCamera.Position(mXPlayer.Position()
								+ CVector(0.0f, 2.0f, 0.0f));

	mActionCamera.Update();
	mActionCamera.Render();
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//頂点にアニメーションを適用する
	//mModelX.AnimateVertex();
	//モデル描画
	//mModelX.Render();
	mpPaladin->Render();
	mXPlayer.Render();
	//敵描画
	mXEnemy.Render();
	//コライダの描画
	CCollisionManager::Instance()->Render();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMMING");

	//2Dの描画終了
	CCamera::End();
}