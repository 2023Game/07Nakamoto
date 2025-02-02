#include "CGhost.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",		true,	59.0f,	1.0f	},	// 待機
};

// コンストラクタ
CGhost::CGhost()
	: mpBattleTarget(nullptr)
{
	// 敵を初期化
	InitEnemy("Ghost", &ANIM_DATA);

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
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eAttackCol });

}

// デストラクタ
CGhost::~CGhost()
{
}

// 攻撃中か
bool CGhost::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CGhost::AttackStart()
{
}

// 攻撃終了
void CGhost::AttackEnd()
{
}

// 衝突処理
void CGhost::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

}

// 更新
void CGhost::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
		// 追いかける
	case EState::eChase:	UpdateChase();	break;
	}

}

// 戦闘相手の方へ向く
void CGhost::LookAtBattleTarget(bool immediate)
{
}

// 状態切り替え
void CGhost::ChangeState(int state)
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

void CGhost::UpdateIdle()
{
	// 通常時の待機
	ChangeAnimation((int)EAnimType::eIdle);

	// 敵のベースクラスの更新
	CEnemy::Update();
}

// 追いかける時の更新処理
void CGhost::UpdateChase()
{
	//mMoveSpeed = CVector::zero;

	//// 現在地と目的地を取得
	//CVector pos = Position();
	//CVector targetPos = mpBattleTarget->Position();
	//targetPos.Y(pos.Y());
	//// 現在地から目的地までのベクトルを求める
	//CVector vec = targetPos - pos;
	//// 攻撃範囲内であれば
	//float dist = vec.Length();
	//if (dist <= ATTACK_RANGE)
	//{
	//	// 攻撃状態へ移行
	//	ChangeState((int)EState::eAttack1);
	//}
	//// 攻撃範囲外
	//else
	//{
	//	// 歩行アニメーションを再生
	//	ChangeAnimation((int)EAnimType::eWalk);

	//	// 残り距離が移動距離より大きい場合は、移動距離分移動
	//	CVector dir = vec.Normalized();
	//	float moveDist = CHAISE_SPEED * Times::DeltaTime();
	//	if (dist > moveDist)
	//	{
	//		mMoveSpeed = dir * moveDist;
	//	}
	//	// 残り距離の方が小さい場合は、
	//	// 残り距離分移動して、待機状態へ移行
	//	else
	//	{
	//		mMoveSpeed = dir * dist;
	//		ChangeState((int)EState::eIdle);
	//	}
	//}

	//// 徐々に戦闘相手の方向へ向く
	//LookAtBattleTarget();

}
