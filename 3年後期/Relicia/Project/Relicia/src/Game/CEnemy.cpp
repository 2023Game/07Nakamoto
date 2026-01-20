#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CDebugFieldOfView.h"
#include "Maths.h"
#include "CField.h"
#include "CAdventurer.h"

#define GRAVITY 0.0625f

#define ROTATE_SPEED	6.0f	// 回転速度
#define FOV_HEIGHT		10.0f	// 視野範囲の高さ

#define FOV_ANGLE 45.0f		// 視野範囲の角度
#define FOV_LENGTH 100.0f	// 視野範囲の距離

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
	, mpHpGauge(nullptr)
	, mAttackIndex(0)
	, mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mpLostPlayerNode(nullptr)
	, mLostElapsedTime(0.0f)
	, mpNearNode(nullptr)
	, mpCurrentNode(nullptr)
	//, mpCurrentPatrolRoute(nullptr)
	//, mpPatrolStartPoint(nullptr)
	//, mNextPatrolIndex(0)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
{
	// HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);

#if _DEBUG
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
#endif

}

// デストラクタ
CEnemy::~CEnemy()
{
	// コライダー削除
	SAFE_DELETE(mpBodyCol);

	// HPゲージが存在したら、一緒に削除する
	if (mpHpGauge != nullptr)
	{
		mpHpGauge->SetOwner(nullptr);
		mpHpGauge->Kill();
	}

#if _DEBUG
	// 視野範囲のデバッグ表示の削除
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
#endif

}

// オブジェクト削除を伝える関数
void CEnemy::DeleteObject(CObjectBase* obj)
{
	// 削除されたのがHPゲージであれば、
	// HPゲージのポインタを空にする
	if (obj == mpHpGauge)
	{
		mpHpGauge = nullptr;
	}

#if _DEBUG
	// 削除されたオブジェクトが視野範囲のデバッグ表示であれば、
	// 視野範囲のデバッグ表示のポインタを空にする
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
#endif
}

// 攻撃中か
bool CEnemy::IsAttacking() const
{
	// 攻撃中
	if (mState == EState::eAttack) return true;

	// 攻撃中でない
	return false;
}

// 敵の初期化
void CEnemy::InitEnemy(std::string path, const std::vector<AnimData>* pAnimData)
{
	// アニメーションデータテーブルを設定
	mpAnimData = pAnimData;

	// モデルデータを読み込み
	CModelX* model = CResourceManager::Get<CModelX>(path);

	// テーブル内のアニメーションデータを読み込み
	int size = mpAnimData->size();
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = (*mpAnimData)[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}

	// CXCharacterの初期化
	Init(model);
}

// 指定した位置まで移動する
bool CEnemy::MoveTo(const CVector& targetPos, float speed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ移動
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 今回の移動距離を求める
	float moveDist = speed * Times::DeltaTime();
	// 目的地までの残りの距離を求める
	float remainDist = vec.Length();
	// 残りの距離が移動距離より短い場合
	if (remainDist <= moveDist)
	{
		// 目的地まで移動する
		pos = CVector(targetPos.X(), pos.Y(), targetPos.Z());
		Position(pos);
		return true;	// 目的地に到着したので、trueを返す
	}
	// 残りの距離がキャラクターの半径より短い場合
	else if (remainDist <= GetBodyRadius())
	{
		// そこで移動を終了とする
		return true;
	}

	// 残りの距離が移動距離より長い場合は、
	// 移動距離分目的地へ移動する
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体が衝突した
	if (self == mpBodyCol)
	{
		// フィールドと衝突した
		if (other->Layer() == ELayer::eFloor)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// 壁と衝突した場合
		else if (other->Layer() == ELayer::eWall)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}

		// プレイヤーと衝突した場合
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// アニメーション切り替え
void CEnemy::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// 指定したターゲットが視野範囲内に入ったかどうか
bool CEnemy::IsFoundTarget(CObjectBase* target) const
{
	// ターゲット座標を取得
	CVector targetPos = target->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からターゲットまでのベクトルを求める
	CVector  vec = targetPos - pos;

	vec.Y(0.0f);	//ターゲットとの高さの差を考慮しない

	// ① 視野角度内か求める
	// ベクトルを正規化して長さを1にする
	CVector dir = vec.Normalized();
	// 自身の正面方向ベクトルを取得
	CVector forward = VectorZ();
	// ターゲットまでのベクトルと
	// 自身の正面方向ベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と視野角度で、視野範囲内か判断する
	if (dot < cosf(angleR))	return false;

	// ② 視野距離内か求める
	//ターゲットまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// ターゲットとの間に遮蔽物がないかチェックする
	if (!IsLookTarget(target)) return false;

	//全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からターゲットが見えているかどうか
bool CEnemy::IsLookTarget(CObjectBase* target) const
{
	CVector targetPos = target->Position();
	CVector selfPos = Position();

	// ターゲットの高さで視野範囲内か判定
	float diffY = abs(targetPos.Y() - selfPos.Y());
	if (diffY >= FOV_HEIGHT) return false;

	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, 1.0f, 0.0f);
	// ターゲットの座標を取得
	targetPos += offsetPos;
	// 自分自身の座標を取得
	selfPos += offsetPos;
	
	CHitInfo hit;
	//フィールドとレイ判定を行い、遮蔽物が存在した場合は、ターゲットが見えない
	if (field->CollisionRay(selfPos, targetPos, &hit)) return false;

	// ターゲットとの間に遮蔽物がないので、ターゲットが見えている
	return true;
}

// プレイヤーを攻撃するか確認
bool CEnemy::CheckAttackPlayer()
{
	CAdventurer* adventurer = CAdventurer::Instance();

	// プレイヤーが死亡していたら、追跡対象としない
	if (adventurer->IsDeath()) return false;

	// プレイヤーが視界範囲に入ったら追跡状態にする
	if (IsFoundTarget(adventurer))
	{
		// 戦闘相手（キャラクター）を設定
		mpBattleTarget = adventurer;
		ChangeState(EState::eChase);
		return true;
	}

	return false;
}

// 状態切り替え
void CEnemy::ChangeState(EState state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数の初期化
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機状態の更新処理
void CEnemy::UpdateIdle()
{
}

// 最寄りのノードに移動
void CEnemy::UpdateJoinNavGraph()
{
	if (mpNearNode == nullptr)
	{
		mpNearNode = CNavManager::Instance()->FindNearestNode(mPosition);
	}

	if (CheckAttackPlayer())
	{
		return;
	}

	if (MoveTo(mpNearNode->GetPos(), GetMoveSpeed()))
	{
		mpCurrentNode = mpNearNode;
		mpNearNode = nullptr;
		ChangeState(EState::eIdle());
	}
}

// 巡回中の更新処理
void CEnemy::UpdatePatrol()
{
}

// 追いかける時の更新処理
void CEnemy::UpdateChase()
{
}

// 追いかける時の更新処理
void CEnemy::UpdateLost()
{
}

void CEnemy::UpdateAttack(int index)
{
}

// 仰け反り状態の更新処理
void CEnemy::UpdateHit()
{
}

// 死亡状態の更新処理
void CEnemy::UpdateDeath()
{
}

// 更新
void CEnemy::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
		// 巡回に戻る状態
	case EState::eJoinNavGraph:	UpdateJoinNavGraph();	break;
		// 巡回状態
	case EState::ePatrol:	UpdatePatrol(); break;
		// 追いかける
	case EState::eChase:	UpdateChase();	break;
		// プレイヤーを見失う
	case EState::eLost:		UpdateLost();	break;
		// 攻撃
	case EState::eAttack:	UpdateAttack(mAttackIndex); break;
		// 仰け反り
	case EState::eHit:		UpdateHit();	break;
		// 死亡状態
	case EState::eDeath:	UpdateDeath();	break;
	}

	// 重力
	mMoveSpeedY -= GRAVITY;

	// 移動
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	mIsGrounded = false;

	// HPゲージを更新
	mpHpGauge->Position(Position() + mGaugeOffsetPos);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);

#if _DEBUG
	mpDebugFov->SetColor(GetStateColor(mState));
#endif
}

// 後更新
void CEnemy::LateUpdate()
{
	CXCharacter::LateUpdate();
}

// 描画
void CEnemy::Render()
{
	CXCharacter::Render();
}

#if _DEBUG
CColor CEnemy::GetStateColor(EState state) const
{
	switch (state)
	{
	case CEnemy::EState::eIdle:		return CColor::white;
	case CEnemy::EState::eJoinNavGraph:	return CColor::green;
	case CEnemy::EState::ePatrol:	return CColor::green;
	case CEnemy::EState::eChase:	return CColor::red;
	case CEnemy::EState::eLost:		return CColor::yellow;
	case CEnemy::EState::eAttack:	return CColor::magenta;
	}

	return CColor::white;
}
#endif 
