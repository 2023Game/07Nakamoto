#include "CBullet.h"
#include "CCollisionManager.h"
#include <stdio.h>

//static変数の定義
CModel CBullet::mModelBullet;

//デフォルトコンストラクタ
CBullet::CBullet()
	: mLife(50)
	, mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.3f), 0.5f)
{

}

//衝突処理
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE:	//球コライダの時
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o)) 
		{
			if (o->GetParent()->GetTag() == CCharacter::ETag::ETARGET)
			{
				//衝突しているときは無効にする
				mEnabled = false;

				printf("TargetにHit\n");
			}
			else if (o->GetParent()->GetTag() == CCharacter::ETag::ESWITCH)
			{
				//衝突しているときは無効にする
				mEnabled = false;

				printf("SwitchにHit\n");
			}
		}
		break;
	case CCollider::EType::ETRIANGLE:	//三角コライダの時
		CVector adjust;	//調整値
		//コライダのmとoが衝突しているか判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			if (o->GetParent() != nullptr)
			{
				if (o->GetParent()->GetTag() == CCharacter::ETag::ESWITCH)
				{
					printf("SWitchタイプ");
				}
			}
			
			//衝突しているときは無効にする
			mEnabled = false;

			printf("三角コライダにhit\n");
		}
		break;
	}
}

//更新
void CBullet::Update()
{
	//生存時間の判定
	if (mLife-- > 0) 
	{
		CTransform::Update();
		//位置更新
		mPosition = mPosition + CVector(0.0f, 0.0f, 2.0f) * mMatrixRotate;
	}
	else
	{
		//無効にする
		mEnabled = false;
	}
}

//衝突処理
void CBullet::Collision()
{
	//コライダの優先度の変更
	mCollider.ChangePriority();
	//衝突処理の実行
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

//弾のモデルを取得する
CModel* CBullet::GetModelBullet()
{
	return &mModelBullet;
}
