#include "CPlayer2.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"

// プレイヤーのインスタンス
CPlayer2* CPlayer2::spInstatnce = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",											true,	0.0f	},	// Tポーズ
	{ "Character\\Player2\\anim\\pico_idle.x",		true,	181.0f	},	// 待機
	{ "Character\\Player2\\anim\\pico_walk_s.x",	true,	60.0f	},	// 歩行
	{ "Character\\Player2\\anim\\pico_running.x",	true,	44.0f	},	// 走行
	{ "Character\\Player2\\anim\\pico_jump_start.x",false,	25.0f	},	// ジャンプ開始
	{ "Character\\Player2\\anim\\pico_jumping.x",	false,	 1.0f	},	// ジャンプ中
	{ "Character\\Player2\\anim\\pico_jump_end.x",	false,	26.0f	},	// ジャン終了プ

	{ "Character\\Player2\\anim\\pico_crawl.x",		true,	55.0f	},	// 這う
	{ "Character\\Player2\\anim\\pico_sneak.x",		true,	51.0f	},	// しゃがみ移動
	{ "Character\\Player2\\anim\\pico_crouch_and_pick_up.x",		true,	180.0f	},	// しゃがんで拾う
	
};

#define PLAYER_HEIGHT_CCOL1	12.0f		// プレイヤーの高さ
#define PLAYER_HEIGHT_CCOL2	2.0f		// プレイヤーの高さ
#define PLAYER_WIDTH_CCOL	2.5f		// プレイヤーの幅

#define MOVE_SPEED		0.375f * 2.0f	// 歩く速度
#define RUN_SPEED		1.0f			// 走る速度

#define JUNP_MOVE_DIST	20.0f			// ジャンプ時の移動距離
#define JUNP_MOVE_START	16.0f			// ジャンプ時の移動開始フレーム
#define JUNP_MOVE_END	33.0f			// ジャンプ時の移動終了フレーム
#define JUMP_SPEED		1.0f			// ジャンプの高さ
#define GRAVITY			0.0625f			// 重力

// コンストラクタ
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
	, mHp(100)
	, mSt(100)
{
	// インスタンスの設定
	spInstatnce = this;

	// モデルデータの取得
	CModelX* model = CResourceManager::Get<CModelX>("Player2");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// カプセルコライダ―生成
	mpColliderCapsule = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, PLAYER_HEIGHT_CCOL2, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT_CCOL1, 0.0f),
		PLAYER_WIDTH_CCOL
	);
	mpColliderCapsule->SetCollisionLayers({ ELayer::eField, ELayer::eWall});

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);
}

// デストラクタ
CPlayer2::~CPlayer2()
{
	SAFE_DELETE(mpColliderCapsule);

	spInstatnce = nullptr;
}

// インスタンスのポインタの取得
CPlayer2* CPlayer2::Instance()
{
	return spInstatnce;
}

// 更新処理
void CPlayer2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、変更処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			UpdateIdle();
			break;
		// ジャンプ開始
		case EState::eJumpStart:
			UpdateJump();
			break;
		// ジャンプ中
		case EState::eJump:
			UpdateJump();
			break;
		// ジャンプ終了
		case EState::eJumpEnd:
			UpdateJump();
			break;
	}

	//待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJump)
	{
		UpdateMove();
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// キャラクターの更新
	CXCharacter::Update();

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	CDebugPrint::Print("Grounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("State:%s\n", ToString(mState).c_str());

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// 待機処理
void CPlayer2::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// SPACEキーでジャンプ
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eJumpStart;
		}
	}
}

// ジャンプ処理
void CPlayer2::UpdateJump()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：ジャンプ開始アニメーションを再生
		case 0:
			ChangeAnimation(EAnimType::eJumpStart);
			mStateStep++;
			break;
		// ステップ1：ジャンプ時の移動処理
		case 1:
		{
			float frame = GetAnimationFrame();
			mMoveSpeed = CVector::zero;

			if (frame >= JUNP_MOVE_START)
			{
				mMoveSpeedY += JUMP_SPEED;
				mIsGrounded = false;
				mState = EState::eJump;
				mStateStep++;
			}
			break;
		}
		// ステップ2：ジャンプ開始アニメーションの終了待ち
		case 2:
		{
			if (IsAnimationFinished())
			{
				ChangeAnimation(EAnimType::eJumping);
				mStateStep++;
			}
			break;
		}
		// ステップ2：着地待ち
		case 3:
			if (mIsGrounded)
			{
				mMoveSpeed.X(0.0f);
				mMoveSpeed.Z(0.0f);
				ChangeAnimation(EAnimType::eJumpEnd);
				mState = EState::eJumpEnd;
				mStateStep++;
			}
			break;
		// ステップ４：ジャンプ終了待ち 
		case 4:
			if (IsAnimationFinished())
			{
				mState = EState::eIdle;
				mStateStep = 0;
			}
			break;
	}
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayer2::CalcMoveVec() const
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// 上方向ベクトル(設置している場合は、地面の法線)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// カメラの正面方向ベクトルと上方向ベクトルの外積から
		// 横方向の移動ベクトルを求める
		CVector moveSide = CVector::Cross(up, camForward);
		// 横方向の移動ベクトルと上方向ベクトルの外積から
		// 正面方向の移動ベクトルを求める
		CVector moveForward = CVector::Cross(moveSide, up);

		// 求めた各方向の移動ベクトルから、
		// 最終的なプレイヤーの移動ベクトルを求める
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// 移動の更新処理
void CPlayer2::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		if (CInput::Key(VK_SHIFT))
		{
			// スタミナがあれば、
			if (mSt > 0)
			{
				mMoveSpeed += move * RUN_SPEED;
				mSt--;
			}
			// スタミナがない状態で走ったら、
			else
			{

			}
		}
		else
		{
			mMoveSpeed += move * MOVE_SPEED;
			mSt++;
		}
		
		// 待機状態であれば、
		if (mState == EState::eIdle)
		{
			if (CInput::Key(VK_SHIFT))
			{
				// 走行アニメーションに切り替える
				ChangeAnimation(EAnimType::eRun);
			}
			else
			{
				// 歩行アニメーションに切り替え
				ChangeAnimation(EAnimType::eWalk);
			}
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// 衝突判定
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderCapsule)
	{
		// フィールドと天井の当たり判定処理
		if (other->Layer() == ELayer::eField)
		{
			CVector adjust = hit.adjust;
			// 押し戻しベクトルの分、座標を移動
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
		// 壁との当たり判定処理
		else if (other->Layer() == ELayer::eWall)
		{
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			// 押し戻しベクトルの分、座標を移動
			Position(Position() + adjust * hit.weight);
		}
	}
}

// 描画処理
void CPlayer2::Render()
{
	CXCharacter::Render();
}

// アニメーションの切り替え
void CPlayer2::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}

// 今の状態を画面に表示
std::string CPlayer2::ToString(EState state)
{
	switch (mState)
	{
	case CPlayer2::EState::eTPose:		return "eTPose";
	case CPlayer2::EState::eIdle:		return "eIdle";
	case CPlayer2::EState::eWalk:		return "eWalk";
	case CPlayer2::EState::eRun:		return "eRun";
	case CPlayer2::EState::eJumpStart:	return "eJumpStart";
	case CPlayer2::EState::eJump:		return "eJump";
	case CPlayer2::EState::eJumpEnd:	return "eJumpEnd";
	case CPlayer2::EState::eCrawl:		return "eCrawl";
	case CPlayer2::EState::eSneak:		return "eSneak";
	case CPlayer2::EState::eCrouch_up:	return "eSneak";
	}
}
