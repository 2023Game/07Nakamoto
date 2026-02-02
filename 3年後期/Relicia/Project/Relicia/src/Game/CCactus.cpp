#include "CCactus.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CCactusNeedle.h"
#include "CNavNode.h"

#define ANIM_PATH "Character\\Enemy\\Cactus\\Anim\\"
#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f
#define GAUGE_OFFSET_Y 15.0f
#define DEATH_WAIT_TIME 3.0f
#define ATTACK_RANGE 15.0f
#define ATTACK_START_FRAME 20.0f
#define ATTACK_END_FRAME 30.0f
#define ATTACK_COL_RADIUS 6.0f
#define ATTACK_COL_POS CVector(0.0f, 2.5f, 5.0f)
#define MOVE_SPEED 20.0f
#define CHAISE_SPEED 20.0f
#define LOOKAT_SPEED 90.0f
#define BATTLE_IDLE_TIME_MIN 2.0f
#define BATTLE_IDLE_TIME_MAX 5.0f

#define ATTACK2_DIST 50.0f			// 針攻撃を行う距離
#define ATTACK2_PROB 70				// 針攻撃を行う確率（パーセント）
#define ATTACK2_START_FRAME 30.0f	// 針発射開始フレーム
#define ATTACK2_END_FRAME 42.0f		// 針発射終了フレーム
// 針発射位置のオフセット座標
#define ATTACK2_NEEDLE_OFFSET_POS CVector(0.0f, 5.0f, 10.0f)
#define ATTACK2_NEEDLE_SPEED 100.0f	// 針の速度
#define ATTACK2_NEEDLE_DIST 200.0f	// 針が移動できる距離
#define ATTACK2_NEEDLE_SHOT_COUNT 3	// 針を発射する回数
#define ATTACK2_NEEDLE_SHOT_DIR_COUNT 3	// 針を発射する方向の数
#define ATTACK2_NEEDLE_SHOT_ANGLE 15.0f	// 針を発射する角度

#define ATTACK_WAIT_TIME 1.0f	// 攻撃終了時の待ち時間

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	40.0f,	1.0f	},	// 待機
	{ ANIM_PATH"idle_battle.x",	true,	36.0f,	1.0f	},	// 戦闘中の待機
	{ ANIM_PATH"walk.x",		true,	62.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"attack.x",		false,	42.0f,	1.0f	},	// パンチ攻撃
	{ ANIM_PATH"attack2.x",		false,	52.0f,	1.0f	},	// 針攻撃
	{ ANIM_PATH"hit.x",			false,	46.0f,	1.0f	},	// 仰け反り
	{ ANIM_PATH"death.x",		false,	46.0f,	1.0f	},	// 死亡
};

// コンストラクタ
CCactus::CCactus()
	: mpAttack1Col(nullptr)
	, mShotNeedleCount(0)
{
	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	// 敵を初期化
	InitEnemy("Cactus", &ANIM_DATA);

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
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer, ETag::eSlash });
	mpBodyCol->SetCollisionLayers
	(
		{ 
			ELayer::eFloor, ELayer::eWall, ELayer::ePlayer, ELayer::eAttackCol, ELayer::eMoveCrate
		}
	);

	// 攻撃コライダーを作成
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// プレイヤーの本体コライダーとのみヒットするように設定
	mpAttack1Col->SetCollisionTags({ ETag::ePlayer, ETag::eField });
	mpAttack1Col->SetCollisionLayers({ ELayer::ePlayer, ELayer::eMoveCrate });
	// 攻撃コライダーの座標を設定
	mpAttack1Col->Position(ATTACK_COL_POS);
	// 攻撃コライダーを最初はオフにしておく
	mpAttack1Col->SetEnable(false);
}

// デストラクタ
CCactus::~CCactus()
{
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttack1Col);
}

// 攻撃開始
void CCactus::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CEnemy::AttackStart();

	// パンチ攻撃中であれば、パンチ攻撃のコライダーをオンにする
	if (mAttackIndex == (int)EAttackID::ePunch)
	{
		mpAttack1Col->SetEnable(true);
	}
}

// 攻撃終了
void CCactus::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemy::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttack1Col->SetEnable(false);
}

// 衝突処理
void CCactus::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
			chara->TakeDamage(10, this);
			// 攻撃ヒット済みリストに登録
			AddAttackHitObj(chara);
		}
	}
}

float CCactus::GetMoveSpeed() const
{
	return MOVE_SPEED;
}

// カプセルコライダーの半径を取得
float CCactus::GetBodyRadius() const
{
	return BODY_RADIUS;
}

// 針を発射
void CCactus::ShotNeedle()
{
	CVector pos = Position() + Rotation() * ATTACK2_NEEDLE_OFFSET_POS;
	CVector forward = VectorZ();
	float startAngle = -ATTACK2_NEEDLE_SHOT_ANGLE;
	float endAngle = ATTACK2_NEEDLE_SHOT_ANGLE;
	for (int i = 0; i < ATTACK2_NEEDLE_SHOT_DIR_COUNT; i++)
	{
		float alpha = (float)i / (ATTACK2_NEEDLE_SHOT_DIR_COUNT - 1);
		float angle = Math::Lerp(startAngle, endAngle, alpha);
		CVector dir = CQuaternion(0.0f, angle, 0.0f) * forward;

		CCactusNeedle* needle = new CCactusNeedle(ATTACK2_NEEDLE_SPEED, ATTACK2_NEEDLE_DIST);
		needle->Position(pos);
		needle->Rotation(CQuaternion::LookRotation(dir));
	}
}

// 状態切り替え
void CCactus::ChangeState(EState state)
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
void CCactus::UpdateIdle()
{
	// 通常時の待機
	if (!mIsBattle)
	{
		// 通常時の待機
		if (!mIsBattle)
		{
			// 目的地のノードが無ければ、
			if (!mpCurrentNode && !mpNearNode)
			{
				// 最寄りのノードに移動
				ChangeState(EState::eJoinNavGraph);
			}
			// 巡回ノードがあれば、
			if (mpCurrentNode != nullptr)
			{
				// 巡回状態に移行
				ChangeState(EState::ePatrol);
			}
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
							mAttackIndex = (int)EAttackID::eNeedle;
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
void CCactus::UpdateJoinNavGraph()
{
	// 進路を塞いでいるオブジェクトを攻撃するか
	if (TryAttackBlockingObj())
	{
		mAttackIndex = (int)EAttackID::ePunch;
		return;
	}

	ChangeAnimation((int)EAnimType::eWalk);

	CEnemy::UpdateJoinNavGraph();
}

// 巡回中の更新処理
void CCactus::UpdatePatrol()
{
	// 進路を塞いでいるオブジェクトを攻撃するか
	if (TryAttackBlockingObj())
	{
		mAttackIndex = (int)EAttackID::ePunch;
		return;
	}
	
	ChangeAnimation((int)EAnimType::eWalk);

	CEnemy::UpdatePatrol();
}

// 追いかける時の更新処理
void CCactus::UpdateChase()
{
	// 進路を塞いでいるオブジェクトを攻撃するか
	if (TryAttackBlockingObj())
	{
		mAttackIndex = (int)EAttackID::ePunch;
		return;
	}

	CEnemy::UpdateChase();

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
		mAttackIndex = (int)EAttackID::ePunch;
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
void CCactus::UpdateAttack(int index)
{
	switch (index)
	{
	case 0:
		// パンチ攻撃
		UpdatePunch();
		break;

	case 1:
		// 棘攻撃
		UpdateNeedle();
		break;
	}
}

// パンチ攻撃時の更新処理
void CCactus::UpdatePunch()
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
				mStateStep++;

				//mAttackIndex = (int)EAttackID::None;
				//ChangeState(EState::eIdle);
			}
			break;
			// ステップ４ : 攻撃終了後の待ち時間
		case 4:
			if (mElapsedTime < ATTACK_WAIT_TIME)
			{
				ChangeAnimation((int)EAnimType::eIdle);
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// 戦闘相手が存在しなかった場合
				if (mpBattleTarget == nullptr)
				{
					mAttackIndex = (int)EAttackID::None;
					// 待機状態へ移行
					ChangeState(EState::eIdle);
				}
				// 戦闘相手が存在した場合
				else
				{
					// 戦闘相手が視野範囲内にいた場合は、追跡状態へ移行
					if (IsLookTarget(mpBattleTarget))
					{
						ChangeState(EState::eChase);
					}
					// 戦闘相手が視野範囲内にいなかった場合、見失った状態へ移行
					else
					{
						mpLostPlayerNode->SetPos(mpBattleTarget->Position());
						mpLostPlayerNode->SetEnable(true);
						ChangeState(EState::eLost);
					}
				}
			}
			break;

	}
}

// 針攻撃時の更新処理
void CCactus::UpdateNeedle()
{
	// 攻撃終了まで、徐々に戦闘相手の方向へ向く
	if (mStateStep < 3)
	{
		LookAtBattleTarget();
	}

	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
		case 0:
			ChangeAnimation((int)EAnimType::eAttack2, true);
			mShotNeedleCount = 0;	// 針を発射した回数を初期化
			mStateStep++;
			break;
		// ステップ1：攻撃開始
		case 1:
			// 攻撃開始フレームまで経過したか
			if (GetAnimationFrame() >= ATTACK2_START_FRAME)
			{
				// 攻撃開始処理を呼び出す
				AttackStart();
				mStateStep++;
			}
			break;
		// ステップ2：針発射＆攻撃終了待ち
		case 2:
			// 針を発射した回数が、発射する回数に満たない場合
			if (mShotNeedleCount < ATTACK2_NEEDLE_SHOT_COUNT)
			{
				// 攻撃開始から攻撃終了までの
				// アニメーションフレームの進行度の割合に合わせて
				// 次に針を発射するフレームを求める
				float length = ATTACK2_END_FRAME - ATTACK2_START_FRAME;
				float alpha = (float)mShotNeedleCount / (ATTACK2_NEEDLE_SHOT_COUNT - 1);
				float nextFrame = ATTACK2_START_FRAME + length * alpha;
				// 次に針を発射するフレームを超えた
				if (GetAnimationFrame() >= nextFrame)
				{
					// 針を発射
					ShotNeedle();
					mShotNeedleCount++;
				}
			}
			// 針を全て発射した
			else
			{
				// 攻撃終了フレームまで経過したか
				if (GetAnimationFrame() >= ATTACK2_END_FRAME)
				{
					// 攻撃終了処理を呼び出す
					AttackEnd();
					mStateStep++;
				}
			}
			break;
		// ステップ3：攻撃アニメーション終了待ち
		case 3:
			// アニメーション終了したら、待機状態へ戻す
			if (IsAnimationFinished())
			{
				mAttackIndex = (int)EAttackID::None;
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// 仰け反り状態の更新処理
void CCactus::UpdateHit()
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
void CCactus::UpdateDeath()
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
			}
			break;
	}
}

// 更新
void CCactus::Update()
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
