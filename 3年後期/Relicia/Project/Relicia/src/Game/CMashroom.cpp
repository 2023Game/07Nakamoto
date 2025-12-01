#include "CMashroom.h"
#include "CColliderCapsule.h"
#include "CCrystalObj.h"

#define ANIM_PATH "Character\\Enemy\\Mushroom\\Anim\\"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

#define MAX_HP 1

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	41.0f,	1.0f	},	// 待機
	{ ANIM_PATH"attack1.x",		true,	26.0f,	1.0f	},	// 頭突き攻撃
	{ ANIM_PATH"attack2.x",		true,	26.0f,	1.0f	},	// 回転攻撃

};

// コンストラクタ
CMashroom::CMashroom()
{
	mMaxHp = MAX_HP;
	mHp = 1;

	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, 10.0f, 0.0f);

	// 敵を初期化
	InitEnemy("Mushroom", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// フィールドと、プレイヤーの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::eFloor, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol });
}

// デストラクタ
CMashroom::~CMashroom()
{
}

// ダメージを受ける
void CMashroom::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		//// 仰け反り状態へ移行
		//ChangeState((int)EState::eHit);

		//// 攻撃を加えた相手を戦闘相手に設定
		//mpBattleTarget = causer;

		//// 攻撃を加えた相手の方向へ向く
		//LookAtBattleTarget(true);

		//// 戦闘状態へ切り替え
		//mIsBattle = true;

		//// 移動を停止
		//mMoveSpeed = CVector::zero;
	}
}

// 死亡
void CMashroom::Death()
{
	// 死亡状態に切り替え
	ChangeState((int)EState::eDeath);
}

// 衝突処理
void CMashroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
	//	// 待機状態
	//case EState::eIdle:		UpdateIdle();	break;
	//	// 追いかける
	//case EState::eChase:	UpdateChase();	break;
	//	// 頭突き攻撃
	//case EState::eAttack1:	UpdateAttack1(); break;
	//	// 仰け反り
	//case EState::eHit:		UpdateHit();	break;
	// 死亡状態
	case EState::eDeath:	UpdateDeath();	break;
	}
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);
}

// 更新
void CMashroom::Update()
{
	// 敵のベースクラスの更新
	CEnemy::Update();


}

// 状態切り替え
void CMashroom::ChangeState(int state)
{
	// 攻撃中に他の状態へ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// 状態切り替え
	CEnemy::ChangeState(state);
}

// 死亡状態の更新処理
void CMashroom::UpdateDeath()
{
	//// ステップごとに処理を分ける
	//switch (mStateStep)
	//{
	//	// ステップ0：死亡アニメーション再生
	//case 0:
	//	ChangeAnimation((int)EAnimType::eDeath, true);
	//	mStateStep++;
	//	break;
	//	// ステップ1：アニメーション終了待ち
	//case 1:
	//	// 死亡アニメーションが終了したら、削除
	//	if (IsAnimationFinished())
	//	{
	//		mStateStep++;
	//	}
	//	break;
	//	// ステップ2：死亡後の待ち
	//case 2:
	//	if (mElapsedTime < DEATH_WAIT_TIME)
	//	{
	//		mElapsedTime += Times::DeltaTime();
	//	}
	//	// 待ち時間が終了したら、削除
	//	else
	//	{
	//		Kill();
	//	}
	//	break;
	//}

	new CCrystalObj(ElementType::Fire, Position());

	Kill();

}
