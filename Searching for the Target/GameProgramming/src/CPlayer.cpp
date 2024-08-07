#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"
#include "CColliderLine.h"
#include <math.h>

#include "CColliderHitManager.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Yの回転速度
#define ROTATION_YX CVector(1.0f,0.0f,0.0f)	//Xの回転速度　削除予定
#define VELOCITY CVector(0.0f,0.0f,0.3f)	//移動速度

#define MOS_POS_X 400	//マウス座標のX補正
#define MOS_POS_Y 300	//マウス座標のY補正

#define MOS_POS_FSX 960	//マウス座標のX補正
#define MOS_POS_FSY 540	//マウス座標のY補正

#define AIM_POS 40		//照準の補正値

#define GRAVITY (0.1f)	//重力加速度

CPlayer::CPlayer()
	: mBulletFlag(false)
	, mAir(false)
	, mCursorX(0)
	, mCursorY(0)
{

}

//コンストラクタ
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale, CModel* model)
	: mSphere(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 2.0f)
	, mBulletFlag(false)
	, mAir(false)
	, mCursorX(0)
	, mCursorY(0)
	
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;
}

//更新処理
void CPlayer::Update()
{
	//マウスカーソル座標の取得
	CInput::GetMousePos(&mCursorX, &mCursorY);

	//マウスクリック検出
	if (mInput.Key(VK_LBUTTON))
	{
		//マウス座標コンソールに出力
		//printf("マウスの座標:%d, %d\n", mCursorX, mCursorY);

		//ゲーム画面中心からの座標へ変換する
		mCursorX -= MOS_POS_FSX;
		mCursorY = MOS_POS_FSY - mCursorY;

		//printf("ゲーム画面中心からの座標へ変換する:%d, %d\n", mCursorX, mCursorY);

		//プレイヤーとマウス座標の差を求める
		mCursorX -= mPosition.GetX();
		mCursorY -= mPosition.GetY();

		//printf("プレイヤーとマウス座標の差:%d, %d\n", mCursorX, mCursorY);
	}

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

	//空中にいるとき
	//重力
	if (mAir)
	{
		mPosition = mPosition - CVector(0.0f, GRAVITY, 0.0f);
		printf("空中\n");
	}
	
	mAir = true;

	//スペースかクリックで弾発射
	//長押し入力しても弾が1発しか出ないようにする
	if (mBulletFlag == false)
	{
		//スペース or 左クリックで弾を発射
		if (mInput.Key(WM_LBUTTONDOWN))
		{
			CBullet* bullet = new CBullet();
			bullet->SetModel(CBullet::GetModelBullet());
			bullet->SetScale(CVector(5.0f, 5.0f, 5.0f));
			bullet->SetPosition(CVector(0.0f, 1.75f, 3.0f) * mMatrix);
			bullet->SetRotation(mRotation + CVector(-mCursorY / AIM_POS, -mCursorX / AIM_POS, 0.0f));
			bullet->Update();
			mBulletFlag = true;
		}
	}
	//左クリックが押されてないときフラグをtrueにする
	if (!mInput.Key(WM_LBUTTONDOWN) && mBulletFlag == true)
	{
		mBulletFlag = false;
	}

	//変換行列の更新
	CTransform::Update();
}

//衝突処理
void CPlayer::Collision(CCollider* m, CCollider* o)
{
	//自身のコライダタイプの判定
	switch (m->GetType())
	{
	case CCollider::EType::ESPHERE:	//球コライダ
		//相手のコライダが三角コライダの時
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust, n;	//調整値
			//三角形と球の衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust, &n))
			{
				//当たった三角コライダを可変長配列に格納
				mCollisionManager.AddColliders(o);

				//めり込まない位置に戻す
				mPosition = mPosition + adjust;

				//三角面に垂直な重力
				mPosition = mPosition - CVector(n.GetX() / 10, n.GetY() / 10, n.GetZ() / 10);

				mAir = false;
			}
			//行列の更新
			CTransform::Update();
		}
		break;
	}
}

//衝突処理
void CPlayer::Collision()
{
	//コライダの優先度の変更
	mSphere.ChangePriority();

	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mSphere, COLLISIONRANGE);
}

//コライダの取得
CCollider *CPlayer::GetCollider()
{
	return &mSphere;
}

//static変数の定義
CModel CPlayer::mModelPlayer;
//プレイヤーのモデルを取得する
CModel* CPlayer::GetModelPlayer()
{
	return &mModelPlayer;
}

//static変数の定義
CPlayer* CPlayer::mpInstance = nullptr;
//インスタンスの取得
CPlayer* CPlayer::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CPlayer();
	}
	return mpInstance;
}