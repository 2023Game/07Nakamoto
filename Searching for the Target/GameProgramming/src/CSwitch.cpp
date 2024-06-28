#include "CSwitch.h"
#include "CCollisionManager.h"
#include "CMovingDoor.h"

//デフォルトコンストラクタ
CSwitch::CSwitch()
	: mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mFlag(false)
{
	
}

//スイッチの生成
void CSwitch::SetSwitch(CModel* model, const CVector& pos,
	const CVector& rot, const CVector& scale)
{
	mTag = ETag::ESWITCH;	//タグ
	mpModel = model;		//モデル
	mPosition = pos;		//位置
	mRotation = rot;		//回転
	mScale = scale;			//拡縮

}

//スイッチと壁の設定
void CSwitch::SetSwitch(CSwitch* parent, CModel* model,
	const CVector& spos, const CVector& srot,const CVector& sscale,
	const CVector& dpos, const CVector& drot, const CVector& dscale)
{
	mTag = ETag::ESWITCH;	//タグ
	mpModel = model;		//モデル
	mPosition = spos;		//位置
	mRotation = srot;		//回転
	mScale = sscale;		//拡縮

	//扉の生成
	new CMovingDoor(parent, dpos, drot, dscale);

}

//更新処理
void CSwitch::Update()
{
	//行列の更新
	CTransform::Update();
}

//フラグの取得
bool CSwitch::GetFlag()
{
	return mFlag;
}

//衝突処理
void CSwitch::Collision(CCollider* m, CCollider* o)
{
	switch (o->GetType())
	{
	case CCollider::EType::ESPHERE: //球コライダの時
		//コライダのmとoが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			if (o->GetParent() != nullptr) {
				//タグが弾の時
				if (o->GetParent()->GetTag() == CCharacter::ETag::EBULLET)
				{

					mFlag = true;

					//mEnabled = false;
				}
			}
		}
		break;
	}
}

//衝突処理
void CSwitch::Collision()
{
	//コライダの優先度の変更
	mCollider.ChangePriority();
	//衝突処理の実行
	CCollisionManager::GetInstance()->Collision(&mCollider, COLLISIONRANGE);
}

