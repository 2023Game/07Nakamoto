#include "CTarget.h"
#include "CCollisionManager.h"
#include "CApplication.h"

//移動速度
#define VELOCITY CVector(0.1f,0.0f,0.0f)
//移動範囲
#define RANGE 300

//コンストラクタ
CTarget::CTarget(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale , EState state)
	: count(0)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 0.8f)
{
	mCollider.SetTag(CCollider::ETag::ETARGET);	//タグ
	mpModel = model;		//モデルの設定
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;			//拡縮の設定
	mState = state;			//状態の設定
}

//更新処理
void CTarget::Update()
{
	//行列を更新
	CTransform::Update();

	switch (mState)
	{
	case CTarget::EState::ESTAY:	//滞在

		break;

	case CTarget::EState::EMOVE1:	//横移動

		//位置を移動
		if (count < RANGE / 2)
		{
			mPosition = mPosition + VELOCITY * mMatrixRotate;
			count++;
		}
		else if (count < RANGE * 1.5f)
		{
			mPosition = mPosition - VELOCITY * mMatrixRotate;
			count++;
		}
		else
		{
			count = -RANGE / 2;
		}
		break;
	}
}

//衝突処理
void CTarget::Collision(CCollider* m, CCollider* o)
{
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE:	//球コライダの時
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			//タグがnullptrのコライダーは判定しない
			if (o->GetParent() != nullptr)
			{
				//相手のタグが弾か判定
				if (o->GetTag() == CCollider::ETag::EBULLET)
				{
					//衝突したときは無効にする
					mEnabled = false;

					CApplication::GetUi()->AddPoint();
				}
			}
			break;
		}
	}
}

//衝突処理
void CTarget::Collision()
{
	//コライダの優先度の変更
	mCollider.ChangePriority();
	//衝突処理の実行
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

//static変数の定義
CModel CTarget::mModelTarget;

//的のモデルを取得する
CModel* CTarget::GetModelTarget()
{
	return &mModelTarget;
}