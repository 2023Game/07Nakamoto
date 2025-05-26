#include "CPlayerBase.h"

#define GRAVITY 0.0625f	// 重力加速度

// コンストラクタ
CPlayerBase::CPlayerBase()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeed(CVector::zero)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mGroundNormal(CVector::up)
	, mpBodyCol(nullptr)
{
}

// デストラクタ
CPlayerBase::~CPlayerBase()
{
	// コライダー削除
	SAFE_DELETE(mpBodyCol);
}

// 衝突処理
void CPlayerBase::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体のコライダーの衝突判定
	if (self == mpBodyCol)
	{
		// フィールドとの衝突
		if (other->Layer() == ELayer::eField)
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

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
				}
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
		else if (other->Layer() == ELayer::eWall || other->Layer() == ELayer::eInteractObj)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
		// 敵と衝突した場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// プレイヤーの初期化
void CPlayerBase::InitPlayer(std::string path, const std::vector<AnimData>* pAnimData)
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

// 状態切り替え
void CPlayerBase::ChangeState(int state)
{
	// 同じ状態の場合は切り替えない
	if (state == mState) return;

	// 状態を変更して、状態関連の変数の初期化
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// アニメーション切り替え
void CPlayerBase::ChangeAnimation(int type, bool restart)
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

// 更新
void CPlayerBase::Update()
{
	// 重力
	mMoveSpeedY -= GRAVITY;

	// 移動
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;
}

// 描画
void CPlayerBase::Render()
{
	CXCharacter::Render();
}

