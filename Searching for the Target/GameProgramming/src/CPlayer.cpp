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

#define AIM_POS 10		//照準の補正値

#define GRAVITY (0.1f)	//重力加速度

//デフォルトコンストラクタ
CPlayer::CPlayer()
	: mSphere(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 2.0f)
	//: mLine(this, &mMatrix, CVector(0.0f, 0.0f, 0.5f), CVector(0.0f, 0.0f, -0.5f))
	//, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, -3.0f, 0.0f))
	//, mLine3(this, &mMatrix, CVector(1.1f, 0.0f, 0.0f), CVector(-1.1f, 0.0f, 0.0f))
	//, mLine4(this, &mMatrix, CVector(0.0f, 2.0f, 3.0f), CVector(0.0f, -2.1f, 0.0f))
	, mBulletFlag(nullptr)
	//, mSlopeFlag(false)
	, mCursorX(0)
	, mCursorY(0)
	, mFx(0)
	, mFy(0)
{

}

//コンストラクタ
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale)
	: mBulletFlag(nullptr)
	//, mSlopeFlag(false)
	, mCursorX(0)
	, mCursorY(0)
	, mFx(0)
	, mFy(0)
	
{
	CTransform::Update(pos, rot, scale);	//行列の更新
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
	mCursorX -= MOS_POS_X;
	mCursorY = MOS_POS_Y - mCursorY;

	//プレイヤーとマウス座標の差を求める
	mCursorX -= mPosition.GetX();
	mCursorY -= mPosition.GetY();

	//printf("プレイヤーとマウス座標の差:%d, %d\n", mCursorX, mCursorY);

	}

	//X軸で移動
	if (mCursorX < -100 && mFx > -4.0f)
	{
		//カメラ注視点を左へ移動
		mFx -= 0.1f;
	}
	else if (mCursorX > 100 && mFx < 4.0f)
	{
		//カメラ注視点を右へ移動
		mFx += 0.1f;
	}

	//Y軸で移動
	if (mCursorY < -100 && mFy > -4.0f)
	{
		//カメラ注視点を下へ移動
		mFy -= 0.1f;
	}
	else if (mCursorY > 100 && mFy < 4.0f)
	{
		//カメラ注視点を上へ移動
		mFy += 0.1f;
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

	//確認用 X軸回転(上)
	//if (mInput.Key('K'))
	//{
	//	mRotation = mRotation + ROTATION_YX;
	//}
	//確認用 X軸回転(下)
	//if (mInput.Key('I'))
	//{
	//	mRotation = mRotation - ROTATION_YX;
	//}

	//坂にいるとき重力を消したい
	//重力
	mPosition = mPosition - CVector(0.0f, GRAVITY, 0.0f);

	//スペースかクリックで弾発射
	//長押し入力しても弾が1発しか出ないようにする
	if (mBulletFlag == false)
	{
		//スペース or 左クリックで弾を発射
		if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
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
	if (o == nullptr)
		return;
	
	//自身のコライダタイプの判定
	switch (m->GetType())
	{
	case CCollider::EType::ESPHERE:	//球コライダ
		//相手のコライダが三角コライダの時
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{

			CVector adjust;	//調整値
			//三角形と球の衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				

				/*
				if (mInput.Key('W') || mInput.Key('A') || mInput.Key('S') || mInput.Key('D'))
				{
					//回転値の修正値を格納
					CVector ajustRote;
					//斜面の角度を求める
					CCollider::Slope(m, o, &ajustRote);
					//坂に当たったら斜面に合わせて回転
					SetRotation(ajustRote);
				}
				*/

				//坂のタグがついているか判定
				if (o->GetTag() == CCollider::ETag::ESLOPE || o->GetTag() == CCollider::ETag::ENULL)
				{
					//当たった三角コライダを可変長配列に格納
					mCollisionManager.AddColliders(o);

					//位置の更新
					mPosition = mPosition + adjust;

					//mSlopeFlag = true;
					//回転値の修正値を格納
				//	CVector ajustRote;
					//斜面の角度を求める
				//	CCollider::Slope(m, o, &ajustRote);

				//	printf("%10f %10f %10f\n", ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());


					//if (mRotation.GetX() + ajustRote.GetX() < -1 || mRotation.GetX() + ajustRote.GetX() > 1)
					//{
						//確認用
						//printf("修正前：%10f %10f %10f\n",
						//mRotation.GetX(), mRotation.GetY(), mRotation.GetZ();

					//	mRotation.Set(mRotation.GetX() + ajustRote.GetX() / 10, ajustRote.GetY(), ajustRote.GetZ());
					//}
					//else
					//{
					//坂に当たったら斜面に合わせて回転
				//	SetRotation(ajustRote);
					//}


					//確認用
					//printf("修正値：%10f %10f %10f\n",
					//	ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());
					//printf("修正後：%10f %10f %10f\n",
					//	mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());

					//坂を滑らないようにする
					//mPosition.Set(mPosition.GetX(), mPosition.GetY(), mPosition.GetZ() - 0.1f);
				}
			}

			//if (mSlopeFlag == false)
			//{
				//回転値の修正値を格納
			//	CVector ajustRote;
				//斜面の角度を求める
			//	CCollider::Slope(m, o, &ajustRote);
				//坂に当たったら斜面に合わせて回転
			//	SetRotation(ajustRote);
			//}
			//mSlopeFlag = false;

		//行列の更新
		CTransform::Update();
		}
		break;

	case CCollider::EType::ELINE:	//線コライダ
		//相手のコライダが三角コライダの時
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleLine(o, m, &adjust))
			{

				//printf("HIT");

				//if (m == &mLine || m == &mLine2 || m == &mLine3)
					//位置の更新
				mPosition = mPosition + adjust;

				//if (m == &mLine4)
				//{
					
					//mRotation = mRotation - CVector(-0.1f, 0.0f, 0.0f);
				//}

				/*
				CVector ajustRote;
				//斜面の角度を求める
				CCollider::Slope(m, o, &ajustRote);

				printf("修正前：%10f %10f %10f\n",
					mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());

				//坂に当たったら回転
				//mRotation = mRotation + ajustRote;

				//坂に当たったら斜面に合わせて回転
				SetRotation(ajustRote);

				//printf("修正値：%10f %10f %10f\n",
				//	ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());

				printf("修正後：%10f %10f %10f\n",
					mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());
				*/
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
	//mLine.ChangePriority();
	//mLine2.ChangePriority();
	//mLine3.ChangePriority();
	//mLine4.ChangePriority();

	//衝突処理を実行
	CCollisionManager::GetInstance()->Collision(&mSphere, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine2, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine3, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine4, COLLISIONRANGE);
}

//コライダの取得
CCollider *CPlayer::GetCollider()
{
	return &mSphere;
}

//カーソルのX座標を取得
float CPlayer::GetFx()
{
	return mFx;
}
//カーソルのY座標を取得
float CPlayer::GetFy()
{
	return mFy;
}

/*
//プレイヤーと当たっているコライダを格納する
void CPlayer::AddHitCollider(CCollider* o)
{
	CCollisionManager::GetInstance()->mpHitCollider.push_back(o);
}

//タグの確認
CCollider CPlayer::HitColliderSerch()
{
	for (size_t i = 0; i < CCollisionManager::GetInstance()->mpHitCollider.size(); i++)
	{
		if (CCollisionManager::GetInstance()->mpHitCollider[i]->GetParent()->GetTag() == CCharacter::ETag::ESLOPE)
		{
			return *CCollisionManager::GetInstance()->mpHitCollider[i];
		}
	}
	return *CCollisionManager::GetInstance()->mpHitCollider[0];
}

//可変長配列の削除
void CPlayer::Delete()
{
	//イテレータの生成
	std::vector<CCollider*>::iterator itr;
	//イテレータを先頭へ
	itr = CCollisionManager::GetInstance()->mpHitCollider.begin();
	//最後まで繰り返し
	while (itr != CCollisionManager::GetInstance()->mpHitCollider.end())
	{
		//インスタンスを削除
		delete* itr;
		//配列から削除
		itr = CCollisionManager::GetInstance()->mpHitCollider.erase(itr);
	}
}

CCollider CPlayer::GetCollider()
{
	return mSphere;
}
*/