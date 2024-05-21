#include "CPlayer.h"
#include "CTaskManager.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Yの回転速度
#define VELOCITY CVector(0.0f,0.0f,0.1f) //移動速度
#define VELOCITY_Y CVector(0.0f,2.0f,0.0f) //ジャンプ時の移動量

//ジャンプの仮のフラグ
//bool a = false;

//コンストラクタ
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);	//行列の更新
}

//更新処理
void CPlayer::Update()
{
	//Dキー入力で右回転
	if (mInput.Key('D'))
	{
		//Y軸の回転値を減算
		mRotation = mRotation - ROTATION_YV;
	}
	//Aキー入力で左回転
	if (mInput.Key('A'))
	{
		//Y軸の回転値を加算
		mRotation = mRotation + ROTATION_YV;
	}
	//Wキー入力で前進
	if (mInput.Key('W'))
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	//Sキー入力で後退
	if (mInput.Key('S')) 
	{
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition - VELOCITY * mMatrixRotate;
	}

	/* 衝突判定ができてからジャンプ実装予定
	//SPACEキー入力でジャンプ
	if (a == false) {
		if (mInput.Key(VK_SPACE))
		{
			//Y軸方向の値を移動させる
			mPosition = mPosition + VELOCITY_Y;
			a = true;
		}
	}

	if (a == true && !mInput.Key(VK_SPACE))
	{
		a = false;
	}
	*/

	//左クリック入力で弾発射：三角形の弾
	/*if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
	{
		CBullet* bullet = new CBullet();
		bullet->SetSize(0.2f, 1.5f);
		bullet->SetPosition(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->SetRotation(mRotation);
		bullet->Update();
	}*/

	//変換行列の更新
	CTransform::Update();
}