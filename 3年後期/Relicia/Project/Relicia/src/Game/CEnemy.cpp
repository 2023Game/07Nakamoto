#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "CColliderCapsule.h"
#include "CGaugeUI3D.h"
#include "CNavNode.h"
#include "CNavManager.h"

#define GRAVITY 0.0625f

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
	, mpCurrentPatrolRoute(nullptr)
	, mpPatrolStartPoint(nullptr)
	, mNextPatrolIndex(0)
{
	// HPゲージを作成
	mpHpGauge = new CGaugeUI3D(this);
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurrPoint(mHp);
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
