#include "CTarget.h"

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
	mpModel = model;		//モデルの設定
	mPosition = position;	//位置の設定
	mRotation = rotation;	//回転の設定
	mScale = scale;			//拡縮の設定
	mType = state;
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
			//衝突しているときは無効にする
			mEnabled = false;
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

//更新処理
void CTarget::Update()
{
	//行列を更新
	CTransform::Update();

	switch (mType)
	{
	case CTarget::EState::ESTAY:		//滞在
		
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