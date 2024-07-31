#include "CSwitch.h"
#include "CCollisionManager.h"

//デフォルトコンストラクタ
CSwitch::CSwitch()
	: mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 1.5f)
	, mFlag(false)
{
	mCollider.SetTag(CCollider::ETag::ESWITCH);
}

//スイッチの生成
void CSwitch::SetSwitch(CModel* model, const CVector& pos,
	const CVector& rot, const CVector& scale)
{
	mpModel = model;		//モデル
	mPosition = pos;		//位置
	mRotation = rot;		//回転
	mScale = scale;			//拡縮

	//行列の更新
	CTransform::Update();

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
				if (o->GetTag() == CCollider::ETag::EBULLET)
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

//static変数の定義
CModel CSwitch::mModelSwitch;

//スイッチのモデルを取得する
CModel* CSwitch::GetModelSwitch()
{
	return &mModelSwitch;
}
