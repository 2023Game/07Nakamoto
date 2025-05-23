//プレイヤークラスのインクルード
#include "CPlayer2.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"

#include "CCat.h"
#include "CCamera.h"
#include "CGameCamera2.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Player2\\Rusk\\anim\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

// プレイヤーのインスタンス
CPlayer2* CPlayer2::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer2::AnimData CPlayer2::ANIM_DATA[] =
{
	{ "",						true,	251.0f,	1.0f	},	// 待機
	{ANIM_PATH"walk.x",			true,	31.0f,	0.5f	},	// 歩行

};

// コンストラクタ
CPlayer2::CPlayer2()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mpRideObject(nullptr)
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
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
CPlayer2::~CPlayer2()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
}

// インスタンスを取得
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer2::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// 状態を切り替え
void CPlayer2::ChangeState(EState state)
{
	if (mState == state) return;

	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機
void CPlayer2::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		//// 左クリックで斬撃攻撃へ移行
		//if (CInput::PushKey(VK_LBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack1);
		//}
		//// 右クリックでキック攻撃へ以降
		//else if (CInput::PushKey(VK_RBUTTON))
		//{
		//	mMoveSpeed = CVector::zero;
		//	ChangeState(EState::eAttack2);
		//}
		//// SPACEキーでジャンプ開始へ移行
		//else if (CInput::PushKey(VK_SPACE))
		//{
		//	ChangeState(EState::eJumpStart);
		//}
	}
}

// 斬り攻撃
void CPlayer2::UpdateAttack1()
{
}

// 蹴り攻撃
void CPlayer2::UpdateAttack2()
{
}

// ジャンプ開始
void CPlayer2::UpdateJumpStart()
{
	//ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer2::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		//ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CPlayer2::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

// 仰け反り
void CPlayer2::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
		// 仰け反りアニメーションを開始
		//ChangeAnimation(EAnimType::eHit, true);
		mStateStep++;
		break;
	case 1:
		// 仰け反りアニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			//ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

// オブジェクト削除を伝える
void CPlayer2::DeleteObject(CObjectBase* obj)
{
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayer2::CalcMoveVec()
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	if (CInput::PushKey('C'))
	{
		CCat* cat = CCat::Instance();
		CVector atPos = cat->Position() + CVector(0.0f, 10.0f, 0.0f);
		CCamera* camera = CCamera::MainCamera();

		camera->LookAt(atPos + CVector(0.0f, 0.0f, 40.0f), atPos, CVector::up);
		camera->SetFollowTargetTf(cat);
	}

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
		mMoveSpeed += move * MOVE_SPEED;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eWalk);
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

// 更新
void CPlayer2::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:			UpdateIdle();		break;
		// 斬り攻撃
	case EState::eAttack1:		UpdateAttack1();	break;
		// 蹴り攻撃
	case EState::eAttack2:		UpdateAttack2();	break;
		// ジャンプ開始
	case EState::eJumpStart:	UpdateJumpStart();	break;
		// ジャンプ中
	case EState::eJump:			UpdateJump();		break;
		// ジャンプ終了
	case EState::eJumpEnd:		UpdateJumpEnd();	break;
		// 仰け反り
	case EState::eHit:			UpdateHit();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
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
	//CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	//CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	//CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	//CDebugPrint::Print("PlayerState:%d\n", mState);

	mIsGrounded = false;

	//CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// 攻撃中か
bool CPlayer2::IsAttacking() const
{
	// 斬り攻撃中
	if (mState == EState::eAttack1) return true;
	// 蹴り攻撃中
	if (mState == EState::eAttack2) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CPlayer2::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();
}

// 攻撃終了
void CPlayer2::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();
}

// ダメージを受ける
void CPlayer2::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CXCharacter::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		ChangeState(EState::eHit);

		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 衝突処理
void CPlayer2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
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
void CPlayer2::Render()
{
	CXCharacter::Render();
}
