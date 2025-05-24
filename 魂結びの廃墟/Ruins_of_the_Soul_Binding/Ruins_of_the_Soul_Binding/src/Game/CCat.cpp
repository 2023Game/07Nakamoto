#include "CCat.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Cat\\anim\\"

#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

// 猫のインスタンス
CCat* CCat::spInstance = nullptr;

// 猫のアニメーションデータのテーブル
const std::vector<CPlayerBace::AnimData> ANIM_DATE =
{
		{ "",						true,	0.0f,	1.0f	},	// Tポーズ

};

// コンストラクタ
CCat::CCat()
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// 猫を初期化
	InitPlayer("Cat", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eDoor, ELayer::eEnemy, ELayer::eAttackCol });

}

// デストラクタ
CCat::~CCat()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
}

// インスタンスを取得
CCat* CCat::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CCat::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
}

// 状態を切り替え
void CCat::ChangeState(EState state)
{
	if (mState == (int)state) return;

	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = (int)state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機
void CCat::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{

	}
}

bool CCat::IsAttacking() const
{
	return false;
}

// オブジェクト削除を伝える
void CCat::DeleteObject(CObjectBase* obj)
{
}

// キーの入力情報から移動ベクトルを求める
CVector CCat::CalcMoveVec() const
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
void CCat::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == (int)EState::eIdle)
		{
			//ChangeAnimation(EAnimType::eWalk);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == (int)EState::eIdle)
		{
			//ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// 更新
void CCat::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case (int)EState::eIdle:			UpdateIdle();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == (int)EState::eIdle
		|| mState == (int)EState::eJumpStart
		|| mState == (int)EState::eJump
		|| mState == (int)EState::eJumpEnd)
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

	// ホイールクリックで弾丸発射
	if (CInput::PushKey(VK_MBUTTON))
	{

	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// キャラクターの更新
	CXCharacter::Update();

	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);

	mIsGrounded = false;

	CDebugPrint::Print("FPS:%f\n", Times::FPS());

}

// 攻撃開始
void CCat::AttackStart()
{
}

// 攻撃終了
void CCat::AttackEnd()
{
}

// ダメージを受ける
void CCat::TakeDamage(int damage, CObjectBase* causer)
{
}

// 衝突処理
void CCat::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
		else if (other->Layer() == ELayer::eWall || other->Layer() == ELayer::eDoor)
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

// 描画
void CCat::Render()
{
	CXCharacter::Render();
}
