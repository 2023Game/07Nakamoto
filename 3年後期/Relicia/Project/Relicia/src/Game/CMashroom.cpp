#include "CMashroom.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CCrystalObj.h"
#include "CNavManager.h"
#include "CNavNode.h"
#include "CAdventurer.h"

#define ANIM_PATH "Character\\Enemy\\Mushroom\\Anim\\"
#define BODY_HEIGHT 9.0f
#define BODY_RADIUS 5.0f

#define GAUGE_OFFSET_Y 10.0f
#define DEATH_WAIT_TIME 3.0f
#define ATTACK_RANGE 10.0f
#define ATTACK_START_FRAME 20.0f
#define ATTACK_END_FRAME 30.0f
#define ATTACK_COL_RADIUS 4.0f
#define ATTACK_COL_POS CVector(0.0f, 2.5f, 3.0f)
#define MOVE_SPEED 20.0f
#define CHAISE_SPEED 20.0f
#define LOOKAT_SPEED 5.0f
#define BATTLE_IDLE_TIME_MIN 2.0f
#define BATTLE_IDLE_TIME_MAX 5.0f

#define ATTACK2_DIST 50.0f			// 針攻撃を行う距離
#define ATTACK2_PROB 70				// 針攻撃を行う確率（パーセント）

#define MAX_HP 10

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	41.0f,	1.0f	},	// 待機
	{ ANIM_PATH"idle_battle.x",	true,	36.0f,	1.0f	},	// 戦闘中の待機
	{ ANIM_PATH"walk.x",		true,	60.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"attack1.x",		false,	52.0f,	1.0f	},	// 頭突き攻撃
	{ ANIM_PATH"attack2.x",		false,	52.0f,	1.0f	},	// 回転攻撃
	{ ANIM_PATH"hit.x",			false,	48.0f,	1.0f	},	// 仰け反り
	{ ANIM_PATH"death.x",		false,	52.0f,	1.0f	},	// 死亡
};

// コンストラクタ
CMashroom::CMashroom()
	: mpAttack1Col(nullptr)
{
	mMaxHp = MAX_HP;
	mHp = mMaxHp;

	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	// 敵を初期化
	InitEnemy("Mushroom", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdleBattle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// フィールドと、プレイヤーの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eSlash });
	mpBodyCol->SetCollisionLayers({ ELayer::eFloor, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol });

	// 攻撃コライダーを作成
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// プレイヤーの本体コライダーとのみヒットするように設定
	mpAttack1Col->SetCollisionTags({ ETag::ePlayer });
	mpAttack1Col->SetCollisionLayers({ ELayer::ePlayer });
	// 攻撃コライダーの座標を設定
	mpAttack1Col->Position(ATTACK_COL_POS);
	// 攻撃コライダーを最初はオフにしておく
	mpAttack1Col->SetEnable(false);
}

// デストラクタ
CMashroom::~CMashroom()
{
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttack1Col);
}

// 攻撃開始
void CMashroom::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CEnemy::AttackStart();

	// 頭突き攻撃中であれば、頭突き攻撃のコライダーをオンにする
	if (mAttackIndex == (int)EAttackID::eHeadbutt)
	{
		mpAttack1Col->SetEnable(true);
	}
}

// 攻撃終了
void CMashroom::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemy::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttack1Col->SetEnable(false);
}

// ダメージを受ける
void CMashroom::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		ChangeState(EState::eHit);

		// 攻撃を加えた相手を戦闘相手に設定
		mpBattleTarget = causer;

		// 攻撃を加えた相手の方向へ向く
		LookAtBattleTarget(true);

		// 戦闘状態へ切り替え
		mIsBattle = true;

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 死亡
void CMashroom::Death()
{
	// 死亡状態に切り替え
	ChangeState(EState::eDeath);
}

// 戦闘相手の方へ向く
void CMashroom::LookAtBattleTarget(bool immediate)
{
	// 戦闘相手がいなければ、処理しない
	if (mpBattleTarget == nullptr) return;

	// 戦闘相手までの方向ベクトルを求める
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// すぐに戦闘相手の方向へ向く
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// 徐々に戦闘相手の方向へ向く
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// 移動速度を取得
float CMashroom::GetMoveSpeed() const
{
	return MOVE_SPEED;
}

// カプセルコライダーの半径を取得
float CMashroom::GetBodyRadius() const
{
	return BODY_RADIUS;
}

// 状態切り替え
void CMashroom::ChangeState(EState state)
{
	// 攻撃中に他の状態へ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mState != state && IsAttacking())
	{
		AttackEnd();
		mAttackIndex = (int)EAttackID::None;
	}

	// 状態切り替え
	CEnemy::ChangeState(state);
}

// 待機状態の更新処理
void CMashroom::UpdateIdle()
{
	// プレイヤーを攻撃するか確認
	CheckAttackPlayer();

	// 通常時の待機
	if (!mIsBattle)
	{
		if (!mpCurrentNode && !mpNearNode)
		{
			ChangeState(EState::eJoinNavGraph);
		}
		else
		{
			// 巡回状態に移行
			ChangeState(EState::ePatrol);
			//ChangeAnimation((int)EAnimType::eIdle);
		}
	}
	// 戦闘時の待機
	else
	{
		ChangeAnimation((int)EAnimType::eIdleBattle);
		// 徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// ステップごとに処理を切り替え
		switch (mStateStep)
		{
			// ステップ0：待機時間をランダムで決定
		case 0:
			// 待機時間が決まってなければ
			if (mBattleIdletime <= 0.0f)
			{
				// ランダムで待機時間を決定
				mBattleIdletime = Math::Rand
				(
					BATTLE_IDLE_TIME_MIN,
					BATTLE_IDLE_TIME_MAX
				);
			}
			mStateStep++;
			break;
			// ステップ1：待機時間の経過待ち
		case 1:
			// 戦闘時の待機時間待ち
			if (mElapsedTime < mBattleIdletime)
			{
				mElapsedTime += Times::DeltaTime();
			}
			// 待機時間が経過した
			else
			{
				// 次の状態（デフォルトは追跡状態）
				EState nextState = EState::eChase;

				// 戦闘相手までの距離を求める
				CVector targetPos = mpBattleTarget->Position();
				CVector vec = targetPos - Position();
				vec.Y(0.0f);
				float dist = vec.Length();
				// 戦闘相手までの距離が離れていたら、
				if (dist >= ATTACK2_DIST)
				{
					// 一定確率で、針攻撃に変更
					int rand = Math::Rand(0, 99);
					if (rand < ATTACK2_PROB)
					{
						nextState = EState::eAttack;
						mAttackIndex = (int)EAttackID::eSpin;
					}
				}

				// 次の状態へ移行
				ChangeState(nextState);

				// 戦闘待機時間を初期化
				mBattleIdletime = 0.0f;
			}
			break;
		}
	}
}

// 最寄りのノードに移動
void CMashroom::UpdateJoinNavGraph()
{
	ChangeAnimation((int)EAnimType::eWalk);

	CEnemy::UpdateJoinNavGraph();
}

// 巡回中の更新処理
void CMashroom::UpdatePatrol()
{
	// 攻撃するキャラクターが見つかった場合は、この処理を実行しない
	if (CheckAttackPlayer())
	{
		return;
	}


}

// 追いかける時の更新処理
void CMashroom::UpdateChase()
{
	mMoveSpeed = CVector::zero;

	// 現在地と目的地を取得
	CVector pos = Position();
	CVector targetPos = mpBattleTarget->Position();
	targetPos.Y(pos.Y());
	// 現在地から目的地までのベクトルを求める
	CVector vec = targetPos - pos;
	// 攻撃範囲内であれば
	float dist = vec.Length();
	if (dist <= ATTACK_RANGE)
	{
		// 攻撃状態へ移行
		ChangeState(EState::eAttack);
		mAttackIndex = (int)EAttackID::eHeadbutt;
	}
	// 攻撃範囲外
	else
	{
		// 歩行アニメーションを再生
		ChangeAnimation((int)EAnimType::eWalk);

		// 残り距離が移動距離より大きい場合は、移動距離分移動
		CVector dir = vec.Normalized();
		float moveDist = CHAISE_SPEED * Times::DeltaTime();
		if (dist > moveDist)
		{
			mMoveSpeed = dir * moveDist;
		}
		// 残り距離の方が小さい場合は、
		// 残り距離分移動して、待機状態へ移行
		else
		{
			mMoveSpeed = dir * dist;
			ChangeState(EState::eIdle);
		}
	}

	// 徐々に戦闘相手の方向へ向く
	LookAtBattleTarget();
}

// 攻撃時の更新処理
void CMashroom::UpdateAttack(int index)
{
	switch (index)
	{
	case 0:
		// 頭突き攻撃
		UpdateHeadbutt();
		break;
	case 1:
		// スピン攻撃
		UpdateSpin();
		break;
	}
}

// 頭突き攻撃時の更新処理
void CMashroom::UpdateHeadbutt()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack1, true);
		mStateStep++;
		break;
		// ステップ1：攻撃開始
	case 1:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK_START_FRAME)
		{
			// 攻撃開始処理を呼び出す
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2：攻撃終了
	case 2:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();
			mStateStep++;
		}
		break;
		// ステップ3：攻撃アニメーション終了待ち
	case 3:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// スピン攻撃時の更新処理
void CMashroom::UpdateSpin()
{
	Position(CVector(Position().X(), Position().Y() + 5.0f, Position().Z()));
	ChangeState(EState::eIdle);
	
}

// 仰け反り状態の更新処理
void CMashroom::UpdateHit()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：仰け反りアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eHit, true);
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 仰け反りアニメーションが終了したら、
		// 待機状態へ戻す
		if (IsAnimationFinished())
		{
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// 死亡状態の更新処理
void CMashroom::UpdateDeath()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eDeath, true);
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 死亡アニメーションが終了したら、削除
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// ステップ2：死亡後の待ち
	case 2:
		if (mElapsedTime < DEATH_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待ち時間が終了したら、削除
		else
		{
			Kill();
			new CCrystalObj(ElementType::Fire, Position());
		}
		break;
	}
}

// 衝突処理
void CMashroom::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

	// 攻撃コライダーがヒットした
	if (self == mpAttack1Col)
	{
		// ヒットしたのがキャラクターかつ、
		// まだ攻撃がヒットしていないキャラクターであれば
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// ダメージを与える
			chara->TakeDamage(1, this);
			// 攻撃ヒット済みリストに登録
			AddAttackHitObj(chara);
		}
	}
}

// 更新
void CMashroom::Update()
{
	// 敵のベースクラスの更新
	CEnemy::Update();

#if _DEBUG
	// 戦闘相手までの距離をデバッグ表示
	if (mpBattleTarget != nullptr)
	{
		CVector targetPos = mpBattleTarget->Position();
		CVector pos = Position();
		targetPos.Y(pos.Y());
		float dist = CVector::Distance(targetPos, pos);
		CDebugPrint::Print("Dist:%.2f\n", dist);
	}
#endif // _DEBUG

}
