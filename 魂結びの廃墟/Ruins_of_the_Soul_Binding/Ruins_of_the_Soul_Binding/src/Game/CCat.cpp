#include "CCat.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CPlayer2.h"
#include "CField.h"
#include "CGameUI.h"
#include "CHpGauge.h"
#include "CStGauge.h"
#include "CIcon.h"
#include "CRoom.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Cat\\anim\\"

#define BODY_HEIGHT 12.0f		// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f		// 本体のコライダーの幅
#define MOVE_SPEED 0.75f		// 移動速度
#define RUN_SPEED	1.0f		// 走る速度
#define MOVE_END_DIST 5.0f		// 移動終了する距離
#define JUMP_SPEED 1.0f			// ジャンプ速度d
#define GRAVITY 0.0625f			// 重力加速度
#define ROTATE_SPEED	5.0f	// 移動時の猫の回転速度

#define MAX_HP 100	// 体力の最大値
#define MAX_ST 100	// スタミナの最大値
#define HP_GAUGE_UI_POS 100.0f, 40.0f
#define SP_GAUGE_UI_POS 110.0f, 70.0f

#define EYE_HEIGHT	7.0f		// 視点の高さ

#define TRAIL_SPEED 20.0f		// 追従時の速度
#define TRACKING_DIST 30.0f		// プレイヤーから離れたらついてくる距離

// カメラの回転速度
#define CAMERA_ROT_SPEED 1.5f

// スタミナ増減値(１秒間に増減する値）
#define STAMINA_DELTA 20.0f

// 猫のインスタンス
CCat* CCat::spInstance = nullptr;

// 猫のアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
		{ "",						true,	0.0f,	1.0f	},	// Tポーズ

};

// コンストラクタ
CCat::CCat()
	: CPlayerBase(ETag::eCat)
	, mpTrackingNode(nullptr)
	, mNextTrackingIndex(-1)
	, mMaxSt(MAX_ST)
	, mSt(mMaxSt)
	, mIsDash(false)
{
	// HPの設定
	mMaxHp = MAX_HP;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// 猫を初期化
	InitPlayer("Cat", &ANIM_DATA);

	// 最初の状態を追従状態にする
	ChangeState((int)EState::eTracking);
	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eCat,
		BODY_RADIUS
	);
	mpBodyCol->Position(0.0f, BODY_RADIUS * 0.5f, 0.0f);

	mpBodyCol->SetCollisionTags
	(
		{ 
			ETag::eField,
			ETag::eGimmick,
			ETag::eInteractObject,
			ETag::eRideableObject, 
			ETag::eEnemy,
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{
			ELayer::eFloor, 
			ELayer::eWall, 
			ELayer::eInteractObj,
			ELayer::eEnemy,
			ELayer::eAttackCol,
			ELayer::eDoor,
			ELayer::eSwitch,
		}
	);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::white);

	// 追従時に障害物を避けるためのノードを作成
	mpTrackingNode = new CNavNode(CVector::zero, true);
	mpTrackingNode->SetEnable(false);

	// HPゲージ作成
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	mpHpGauge->SetPos(HP_GAUGE_UI_POS);
	mpHpGauge->SetShow(false);

	// スタミナゲージの作成
	mpStGauge = new CStGauge();
	mpStGauge->SetMaxPoint((int)mMaxSt);
	mpStGauge->SetCurPoint(ceilf(mSt));
	mpStGauge->SetPos(SP_GAUGE_UI_POS);
	mpStGauge->SetShow(false);

}

// デストラクタ
CCat::~CCat()
{
	// コライダーを削除
	//SAFE_DELETE(mpBodyCol);

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		mpNavNode->Kill();
		mpTrackingNode->Kill();
	}
}

// インスタンスを取得
CCat* CCat::Instance()
{
	return spInstance;
}

// 状態を切り替え
void CCat::ChangeState(int state)
{
	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	CPlayerBase::ChangeState(state);
}

// 現在位置からtターゲットが見えているかどうか
bool CCat::IsLookTarget(CObjectBase* target) const
{
	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// ターゲットの座標を取得
	CVector targetPos = target->Position() + offsetPos;
	// 自分自身の座標を取得
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	//フィールドとレイ判定を行い、遮蔽物が存在した場合は、ターゲットが見えない
	if (field->CollisionRay(selfPos, targetPos, &hit)) return false;

	// ターゲットとの間に遮蔽物がないので、ターゲットが見えている
	return true;
}

// 待機
void CCat::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// SPACEキーでジャンプ
		if (CInput::PushKey(VK_SPACE) && IsOperate())
		{
			ChangeState((int)EState::eJumpStart);
		}
	}

	if (!IsOperate())
	{
		if (CInput::PushKey('Q'))
		{
			ChangeState((int)EState::eTracking);
			// 猫と少女のアイコンを設定
			CIcon::Instance()->SetIcon((int)CIcon::EIcon::eTogether);
		}
	}
}

// 追従時の移動経路をけいさんするかどうか
bool CCat::IsCalcTrackingRoute() const
{
	// 経路探索の開始ノードと終了ノードが更新中だったら、移動経路を再計算しない
	if (mpNavNode->IsUpdating()) return false;
	if (mpTrackingNode->IsUpdating()) return false;

	// 移動していない状態
	if (mNextTrackingIndex == -1)
	{
		CPlayer2* player = CPlayer2::Instance();
		// プレイヤーまでの距離を求める
		// 距離の2乗で求める(処理不可が軽いので)
		float playerDist = (player->Position() - Position()).LengthSqr();
		// プレイヤーから一定距離離れると、移動経路を計算する
		return playerDist >= TRACKING_DIST * TRACKING_DIST;	// 距離の2乗と比較するのでTRACKING_DISTも2乗する
	}

	// 現在の移動経路が繋がらなくなっていたら、移動経路を再計算する
	if (!CNavManager::Instance()->IsRouteValid(mTrackingRouteNodes)) return true;

	// 現在位置から次の移動先のノードまで繋がっていなければ、移動経路を再計算する
	CNavNode* nextNode = mTrackingRouteNodes[mNextTrackingIndex];
	if (!mpNavNode->IsConnectNode(nextNode)) return true;

	// 移動経路の計算は不要
	return false;
}

// 追従
void CCat::UpdateTracking()
{
	// 操作フラグがtrueになったら、
	if (IsOperate())
	{
		// 待機状態にする
		ChangeState((int)EState::eIdle);
		mNextTrackingIndex = -1;
		return;
	}

	CPlayer2 *player = CPlayer2::Instance();
	CVector playerPos = player->Position();		// プレイヤーまでの座標

	const auto& trail = player->GetTrail();
	size_t followIndex = 0;

	// 空かどうかを調べる
	if (!trail.empty())
	{
		// 配列の番号で2番目を取得する
		// 2番目まで中身がはいていない場合は、2番目より小さい配列を取得する
		followIndex = min(size_t(2), trail.size() - 1);
	}

	// ついていく座標を設定
	mFollowPos = trail[followIndex];
	// ついていく座標に経路探索用のノードを配置
	mpTrackingNode->SetPos(mFollowPos);
	mpTrackingNode->SetEnable(true);


	// 移動経路を再計算するか
	if (IsCalcTrackingRoute())
	{
		// ついていく座標までの経路を探索する
		CNavManager::Instance()->Navigate(mpNavNode, mpTrackingNode, mTrackingRouteNodes);
		// 移動経路が見つかった
		if (mTrackingRouteNodes.size() >= 2)
		{
			mNextTrackingIndex = 1;
			mStateStep = 0;
		}
		else
		{
			mNextTrackingIndex = -1;
		}
	}

	switch (mStateStep)
	{
		// ステップ0：目標位置まで移動
		case 0:
			// 次に移動するノード番号が設定されていたら
			if (mNextTrackingIndex >= 0)
			{
				// 次に井地王するノードまで移動を行う
				CNavNode* nextNode = mTrackingRouteNodes[mNextTrackingIndex];
				// 移動経路が見つかった場合
				if (MoveTo(nextNode->GetPos(), TRAIL_SPEED))
				{
					// 移動が終われば、次のノードを目的地に変更
					mNextTrackingIndex++;
					// 最終目的地まで移動が終わった
					// (移動先のノード番号が移動経路のリストのサイ以上だったい場合)
					if (mNextTrackingIndex >= mTrackingRouteNodes.size())
					{
						mLookAtPos = playerPos;
						mNextTrackingIndex = -1;
						mStateStep++;
					}
				}
			}
			break;
		// ステップ1：プレイヤーの方向を向く
		case 1:
		{
			CVector targetDir = mLookAtPos - Position();
			targetDir.Y(0.0f);
			targetDir.Normalize();
			// 徐々に移動方向へ移動
			CVector forward = CVector::Slerp
			(
				VectorZ(),	// 現在の正面方向
				targetDir,	// プレイヤーの方向
				ROTATE_SPEED * Times::DeltaTime()
			);
			Rotation(CQuaternion::LookRotation(forward));
			break;
		}
	}

	if (CInput::PushKey('Q'))
	{
		// その場で待機
		ChangeState((int)EState::eIdle);
		// 少女のアイコンを設定
		CIcon::Instance()->SetIcon((int)CIcon::EIcon::ePlayer);
	}
}

// 攻撃中か
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
		CCamera* mainCamera = CCamera::CurrentCamera();
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

	mIsDash = false;
	bool isRecoveryStamina = true;

	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		// 待機状態であれば、
		if (mState == (int)EState::eIdle)
		{
			// スタミナが残っている状態でSHIFTキーを押したら
			if (CInput::Key(VK_SHIFT))
			{
				// ダッシュする
				mIsDash = mSt > 0.0f;
				// スタミナ減少
				mSt -= max(STAMINA_DELTA * Times::DeltaTime(),
					0.0f);

				// スタミナ回復フラグをオフ
				isRecoveryStamina = false;
			}
		}

		mMoveSpeed += move * (mIsDash ? RUN_SPEED : MOVE_SPEED);

		// スタミナが無ければ
		if (mSt < 0.0f)
		{
			mSt = 0.0f;
			mMoveSpeed.X(0.0f);
			mMoveSpeed.Z(0.0f);
		}

	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == (int)EState::eIdle)
		{
			// 移動していなければ、スタミナ回復
			ChangeAnimation((int)EAnimType::eIdle);
		}
	}

	// スタミナ回復フラグが立っていたら
	if (isRecoveryStamina)
	{
		mSt = min(
			mSt + STAMINA_DELTA * Times::DeltaTime(),
			mMaxSt);
	}
}

// 指定した位置まで移動する
bool CCat::MoveTo(const CVector& targetPos, float speed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

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
	else if (remainDist <= MOVE_END_DIST)
	{
		return true;
	}

	// 徐々に移動方向へ移動
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

	// 残りの距離が移動距離より長い場合は、
	// 移動距離分目的地へ移動する
	pos += forward * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
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
	case (int)EState::eIdle:		UpdateIdle();		break;
	// 追従状態
	case(int)EState::eTracking:		UpdateTracking();	break;
	// ジャンプ開始
	case(int)EState::eJumpStart:	UpdateJumpStart();	break;
	// ジャンプ中
	case(int)EState::eJump:			UpdateJump();		break;
	// ジャンプ終了
	case(int)EState::eJumpEnd:		UpdateJumpEnd();	break;
	}

	// このプレイヤーを操作中であれば、
	if (IsOperate())
	{
		// 待機中とジャンプ中は、移動処理を行う
		if (mState == (int)EState::eIdle
			|| mState == (int)EState::eJumpStart
			|| mState == (int)EState::eJump
			|| mState == (int)EState::eJumpEnd)
		{
			UpdateMove();
		}

		// マウスの左右移動で、猫を左右に回転
		CVector2 delta = CInput::GetDeltaMousePos();
		Rotate(0.0f, delta.X() * CAMERA_ROT_SPEED * Times::DeltaTime(), 0.0f);
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
	// 移動
	Position(Position() + moveSpeed);

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// キャラクターの更新
	CPlayerBase::Update();
#if _DEBUG
	// 操作中なら
	if (IsOperate())
	{
		CVector pos = Position();
		CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
		CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
		CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
		CDebugPrint::Print("PlayerState:%d\n", mState);
		CDebugPrint::Print("FPS:%f\n", Times::FPS());
	}

	CDebugPrint::Print("部屋(猫)：%s\n", mpRoom != nullptr ? mpRoom->GetName().c_str() : "なし");
#endif
	// 地面についているか
	mIsGrounded = false;

	// 体力ゲージの更新
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	// スタミナゲージの更新処理
	mpStGauge->SetMaxPoint((int)mMaxSt);
	mpStGauge->SetCurPoint(ceilf(mSt));

}

// 描画
void CCat::Render()
{
	// 猫を操作していたら、頭のない猫の体を描画
	if (mIsOperate)
	{
		// TODO:頭のない猫のモデルデータ
	}
	// 猫を操作してなけらば、全身を描画
	else
	{
		CPlayerBase::Render();
	}
}

// ジャンプ開始
void CCat::UpdateJumpStart()
{
	//ChangeAnimation((int)EAnimType::eJumpStart);
	//float frame = GetAnimationFrame();
	//mMoveSpeed = CVector::zero;

	//if (frame >= JUNP_MOVE_START)
	//{
	//	mMoveSpeedY += JUMP_SPEED;
	//	mIsGrounded = false;
	//	ChangeState((int)EState::eJump);
	//}

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
	ChangeState((int)EState::eJump);
}

// ジャンプ中
void CCat::UpdateJump()
{
	//ChangeAnimation((int)EAnimType::eJump);
	if (mIsGrounded)
	{
		ChangeState((int)EState::eJumpEnd);
	}
}

// ジャンプ終了
void CCat::UpdateJumpEnd()
{
	if (mIsGrounded)
	{
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		//ChangeAnimation((int)EAnimType::eJumpEnd);

		//if (IsAnimationFinished())
		//{
			ChangeState((int)EState::eIdle);
		//}
	}
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
	CPlayerBase::Collision(self, other, hit);
}

void CCat::SetOperate(bool operate)
{
	// ベースクラスの処理を呼び出す
	CPlayerBase::SetOperate(operate);

	// 自身が操作プレイヤーの場合
	if (mIsOperate)
	{
		// 自身を操作する時のUIを表示
		mpHpGauge->SetShow(true);
		mpStGauge->SetShow(true);
	}
	// 自身が操作プレイヤーでない場合
	else
	{
		// 自身が操作する時のUIを非表示
		mpHpGauge->SetShow(false);
		mpStGauge->SetShow(false);
	}

}

const CBounds& CCat::GetBounds() const
{
	return mpBodyCol->Bounds();
}
