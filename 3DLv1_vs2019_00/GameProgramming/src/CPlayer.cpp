#include "CPlayer.h"
#include "CApplication.h"
//#include "CGame.h"

#define TEXCOORD 168, 188, 158, 128	//テクスチャマッピング
#define GRAVITY (TIPSIZE / 20.0f)	//重力加速度
#define JUMPV0 (TIPSIZE / 1.4f)		//ジャンプの初速

//プレイヤークラスのインクルード
#include "CPlayer.h"

#define ROTATION_XV CVector(1.0f,0.0f,0.0f) //回転速度
#define ROTATION_YV CVector(0.0f,1.0f,0.0f) //回転速度
#define VELOCITY CVector(0.0f,0.0f,0.1f) //移動速度

//CPlayer(位置,回転,スケール)
CPlayer::CPlayer(const CVector& pos, const CVector& rot, const CVector& scale)
{
	CTransform::Update(pos, rot, scale); //行列も更新
}

//更新処理
void CPlayer::Update()
{
	//スペースキー入力で弾発射
	if (mInput.Key(VK_SPACE)) {
		bullet.Set(0.1f, 1.5f);
		bullet.Position(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet.Rotation(mRotation);
	}
	//Sキー入力で上向き
	if (mInput.Key('S'))
	{
		//X軸の回転値を減算
		mRotation = mRotation - ROTATION_XV;
	}
	//Wキー入力で上向き
	if (mInput.Key('W'))
	{
		//X軸の回転値を減算
		mRotation = mRotation + ROTATION_XV;
	}
	//Dキー入力で回転
	if (mInput.Key('D'))
	{
		//Y軸の回転地を減少
		mRotation = mRotation - ROTATION_YV;
	}
	//Aキー入力で半時計回転
	if (mInput.Key('A'))
	{
		//Y軸の回転地を増加
		mRotation = mRotation + ROTATION_YV;
	}
	//上キー入力で前進
	if (mInput.Key(VK_UP))
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	//変換行列の更新
	CTransform::Update();
}