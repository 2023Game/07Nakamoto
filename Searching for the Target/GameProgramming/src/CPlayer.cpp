#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Yの回転速度
#define VELOCITY CVector(0.0f,0.0f,0.1f) //移動速度
#define VELOCITY_Y CVector(0.0f,2.0f,0.0f) //ジャンプ時の移動量

#define ROTATION_YX CVector(1.0f,0.0f,0.0f)

//ジャンプの仮のフラグ
//bool a = false;

//デフォルトコンストラクタ
CPlayer::CPlayer()
	: mLine(this, &mMatrix, CVector(0.0f, 0.3f, -1.5f), CVector(0.0f, 0.3f, 1.5f))
	, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -0.2f, 0.0f))
	, mLine3(this, &mMatrix, CVector(1.1f, 0.3f, 0.0f), CVector(-1.1f, 0.3f, 0.0f))
	, mBulletFlag(false)
{

}

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

	//確認用 X軸回転(上)
	if (mInput.Key('K'))
	{
		mRotation = mRotation + ROTATION_YX;
	}
	//確認用 X軸回転(下)
	if (mInput.Key('I'))
	{
		mRotation = mRotation - ROTATION_YX;
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
	
	//スペースかクリックで弾発射
	//長押し入力しても弾が1発しか出ないようにする
	if (mBulletFlag == false)
	{
		//スペース or 左クリックで弾を発射
		if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
		{
			CBullet* bullet = new CBullet();
			bullet->SetModel(CBullet::GetModelBullet());
			bullet->SetScale(CVector(10.0f, 10.0f, 10.0f));
			bullet->SetPosition(CVector(0.0f, 1.75f, 3.0f) * mMatrix);
			bullet->SetRotation(mRotation);
			bullet->Update();
			mBulletFlag = true;
		}
	}
	//スペース or 左クリックが押されてないときフラグをtrueにする
	if (!mInput.Key(VK_SPACE) && !mInput.Key(WM_LBUTTONDOWN) && mBulletFlag == true)
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
	case CCollider::EType::ELINE:	//線コライダ
		//相手のコライダが三角コライダの時
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{
				//位置の更新
				mPosition = mPosition + adjust;
				//行列の更新
				CTransform::Update();
			}
		}
		break;
	}
}

//衝突処理
void CPlayer::Collision()
{
	//コライダの優先度の変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::GetInstance()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::GetInstance()->Collision(&mLine3, COLLISIONRANGE);
}