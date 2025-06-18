//プレイヤークラスのインクルード
#include "CPlayer2.h"
#include "CCat.h"
#include "CInput.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CCamera.h"
#include "CPlayerCamera.h"
#include "CInteractObject.h"
#include "CDebugFieldOfView.h"
#include "CGameUI.h"
#include "CHpGauge.h"
#include "CStGauge.h"
#include "CSceneManager.h"
#include "CDemonPower.h"
#include "CHandGlow.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Player2\\Rusk\\anim\\"

#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define RUN_SPEED	1.0f	// 走る速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度
#define ROTATE_SPEED	5.0f	// 移動時のプレイヤーの回転速度

#define MAX_HP 100	// 体力の最大値
#define MAX_ST 100	// スタミナの最大値
#define DEMON_POWER_DAMAGE	1	// 妖力の源へのダメージ

#define SEARCH_RADIUS	 10.0f		// 調べるオブジェクトを探知する範囲の半径
#define FOV_ANGLE		 60.0f		// 視野範囲の角度
#define FOV_LENGTH		 10.0f		// 視野範囲の距離

#define CHANNELING_TIME	0.5f		// 妖力を流し込んでダメージが入るまでの時間

#define TRAIL_SPEED 15.0f		// 追従時の速度
#define TRACKING_DIST 30.0f		// プレイヤーから離れたらついてくる距離
#define MAX_DISTANCE 5.0f		// 追従する距離を更新する際の距離

// プレイヤーのインスタンス
CPlayer2* CPlayer2::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	251.0f,	1.0f	},	// 待機
	{ANIM_PATH"walk.x",			true,	31.0f,	0.5f	},	// 歩行
	{ANIM_PATH"run.x",			true,	22.0f,	0.5f	},	// 走行
	{ANIM_PATH"hit.x",			false,	30.0f,	1.0f	},	// ダメージを受けた時(仮)
	{ANIM_PATH"died.x",			false,	174.0f,	1.0f	},	// 走行

	{ANIM_PATH"channeling_start.x",	false,	32.0f,	0.5f	},	// 妖力を流し込み開始
	{ANIM_PATH"channeling.x",		true,	9.0f,	0.3f	},	// 妖力を流し込み中
	{ANIM_PATH"channeling_end.x",	false,	39.0f,	0.5f	},	// 妖力を流し込み終了

};

// コンストラクタ
CPlayer2::CPlayer2()
	: mMaxSt(MAX_ST)
	, mSt(mMaxSt)
	, mChannelingTime(0)
#if _DEBUG
	,mpDebugFov(nullptr)
#endif
{
	// HPの設定
	mMaxHp = MAX_HP;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	InitPlayer("Player2", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

#if _DEBUG
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, FOV_LENGTH);
#endif

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags
	(
		{
			ETag::eField,
			ETag::eInteractObject,
			ETag::eRideableObject,
			ETag::eEnemy,
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{
			ELayer::eField,
			ELayer::eWall,
			ELayer::eInteractObj,
			ELayer::eDoor,
			ELayer::eEnemy,
			ELayer::eDemon,
			ELayer::eAttackCol,
		}
	);

	// 調べるオブジェクトを探知するコライダーを作成
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eInteractSearch,
		SEARCH_RADIUS
	);
	// 調べるオブジェクトのみ衝突するように設定
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj });

	// HPゲージ作成
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	mpHpGauge->SetPos(0.0f, 0.0f);
	// スタミナゲージの作成
	mpStGauge = new CStGauge();
	mpStGauge->SetMaxPoint(mMaxSt);
	mpStGauge->SetCurPoint(mSt);
	mpStGauge->SetPos(10.0f, 40.0f);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::red);

	// 追従時に障害物を避けるためのノードを作成
	mpTrackingNode = new CNavNode(CVector::zero, true);
	mpTrackingNode->SetEnable(false);

}

// デストラクタ
CPlayer2::~CPlayer2()
{
#if _DEBUG
	// 視野範囲のデバッグ表示が存在したら、一緒に削除する
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
#endif

	// コライダーを削除
	SAFE_DELETE(mpSearchCol);

	spInstance = nullptr;

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpTrackingNode);
	}
}

// インスタンスを取得
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}

// 状態を切り替え
void CPlayer2::ChangeState(int state)
{
	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	CPlayerBase::ChangeState(state);
}

// 待機
void CPlayer2::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// 近くの調べるオブジェクトを取得
		CInteractObject* obj = GetNearInteractObj();
		if (obj != nullptr)
		{
#if _DEBUG
			// 探知範囲内に入ったオブジェクトの名前を表示
			CDebugPrint::Print
			(
				"%s:%s\n",
				obj->GetDebugName().c_str(),
				obj->GetInteractStr().c_str()
			);
#endif

			// 調べられるオブジェクトの上に調べるUIを表示
			//CGameUI::Instance()->ShowInteractUI(obj);

			// 近くの調べられるオブジェクトが、妖力の源であれば、
			if (CDemonPower* dp = dynamic_cast<CDemonPower*>(obj))
			{
				// 左クリックを押したら妖力を流し込む
				if (CInput::PushKey(VK_LBUTTON))
				{
					mpChannelingDemonPower = dp;
					ChangeState((int)EState::eChanneling);
				}
			}
			else
			{
				// [E]キーを押したら、近くの調べるオブジェクトを調べる
				if (CInput::PushKey('E'))
				{
					if (obj->CanInteract())
					{
						obj->Interact();
					}
				}
			}
		}
		// 近くに調べるオブジェクトがなかった
		else
		{
			// 調べるUIを非表示にする
			//CGameUI::Instance()->HideInteractUI();
		}
	}
}

// 仰け反り
void CPlayer2::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
		// 仰け反りアニメーションを開始
		ChangeAnimation((int)EAnimType::eHit, true);
		mStateStep++;
		break;
	case 1:
		// 仰け反りアニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// 死亡処理
void CPlayer2::UpdateDeath()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// 死亡アニメーションを設定
	ChangeAnimation((int)EAnimType::eDeath);

	// アニメーションが終了したら、
	if (IsAnimationFinished())
	{
		// ゲームオーバーシーンを読み込む
		//CSceneManager::Instance()->LoadScene(EScene::eGameOver);
	}
}

// 妖力を注いでいる
void CPlayer2::UpdateChanneling()
{
	// TODO:アニメーションを開始する距離が近かったら離れた位置に行って処理を開始する

	switch (mStateStep)
	{
	case 0:
		// 妖力を流し込むアニメーションを開始
		ChangeAnimation((int)EAnimType::eChannelingStart);
		mMoveSpeed.Set(0.0f, 0.0f, 0.0f);
		mChannelingTime = 0;
		mStateStep++;
		break;
	case 1:
		// アニメーションが終わったら
		if (IsAnimationFinished())
		{
			// 次のアニメーションへ
			mStateStep++;
		}
		break;
	case 2:
		// 左クリックを押している間
		if (CInput::Key(VK_LBUTTON) && IsOperate())
		{
			// 妖力を流し込むアニメーション
			ChangeAnimation((int)EAnimType::eChanneling);
			mChannelingTime += Times::DeltaTime();

			if (mChannelingTime >= CHANNELING_TIME)
			{
				// 目標にダメージを与える
				mpChannelingDemonPower->TakeDamage(DEMON_POWER_DAMAGE, this);
				// 目標が破壊されたら、次のステップへ
				if (mpChannelingDemonPower->IsDeath())
				{
					mStateStep++;
				}
				mChannelingTime -= CHANNELING_TIME;
			}
		}
		else if (!IsOperate())
		{
			// 妖力を流し込むアニメーション
			ChangeAnimation((int)EAnimType::eChanneling);
			mChannelingTime += Times::DeltaTime();

			if (mChannelingTime >= CHANNELING_TIME)
			{
				// 目標にダメージを与える
				mpChannelingDemonPower->TakeDamage(DEMON_POWER_DAMAGE, this);
				// 目標が破壊されたら、次のステップへ
				if (mpChannelingDemonPower->IsDeath())
				{
					mStateStep++;
				}
				mChannelingTime -= CHANNELING_TIME;
			}
		}
		// マウスの左ボタンを離したら、次のステップへ
		else
		{
			mStateStep++;
		}
		break;
	case 3:
		// 妖力を流し込む目標のポインタを空にする
		mpChannelingDemonPower = nullptr;
		// 妖力の流し込みを終了アニメーションを再生開始
		ChangeAnimation((int)EAnimType::eChannelingEnd);
		mStateStep++;
		break;
	case 4:

		// アニメーションが終わったら
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// 追従状態
void CPlayer2::UpdateTracking()
{
	CCat* cat = CCat::Instance();
	CVector catPos = cat->Position();		// プレイヤーまでのベクトル
	CVector catVec = catPos - Position();	// プレイヤーまでの距離

	// ベクトルの2乗を求める(処理不可が軽いので)
	float playerDist = catVec.LengthSqr();
	// プレイヤーから一定距離離れると、ついていく座標を更新
	if (playerDist >= TRACKING_DIST * TRACKING_DIST)	// ベクトルの2乗と比較するのでTRACKING_DISTも2乗する
	{
		const auto& trail = cat->GetTrail();
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
			ChangeAnimation((int)EAnimType::eIdle);

			mNextTrackingIndex = -1;
			mStateStep = -1;
		}
	}

	switch (mStateStep)
	{
		// ステップ0：目標位置まで移動
	case 0:
		// 次に移動するノード番号が設定されていたら
		if (mNextTrackingIndex >= 0)
		{
			ChangeAnimation((int)EAnimType::eWalk);

			// 次に移動するノードまで移動を行う
			CNavNode* nextNode = mTrackingRouteNodes[mNextTrackingIndex];
			// 移動経路が見つかった場合
			if (MoveTo(nextNode->GetPos(), TRAIL_SPEED))
			{
				

				// 移動が終われば、次のノードを目的地に変更
				mNextTrackingIndex++;
				// 最終目的地まで移動が終わった
				// (移動先のノード番号が移動経路のリストのサイズ以上だった場合)
				if (mNextTrackingIndex >= mTrackingRouteNodes.size())
				{
					mLookAtPos = catPos;
					mStateStep++;
				}
			}

			if (MoveTo(mFollowPos, TRAIL_SPEED))
			{
				mLookAtPos = catPos;
				mStateStep++;
			}

		}
		break;
		// ステップ1：プレイヤーの方向を向く
	case 1:
		{
			ChangeAnimation((int)EAnimType::eWalk);

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

	// 操作フラグがtrueになったら、
	if (IsOperate())
	{
		// 待機状態にする
		ChangeState((int)EState::eIdle);
	}

}

// オブジェクト削除を伝える
void CPlayer2::DeleteObject(CObjectBase* obj)
{
#if _DEBUG
	// 削除されたのが視や表示用のクラスであれば、
	// ポインタを空にする
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
#endif
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
		// 待機状態であれば、
		if (mState == (int)EState::eIdle)
		{
			if (CInput::Key(VK_SHIFT))
			{
				// スタミナがあれば、
				if (mSt > 0)
				{
					// 走行アニメーションに切り替える
					ChangeAnimation((int)EAnimType::eRun);

					mMoveSpeed += move * RUN_SPEED;
					mSt--;
				}
				else
				{
					// 転倒
					//ChangeState((int)EState::eFall);
				}
			}
			else
			{
				// 歩行アニメーションに切り替え
				ChangeAnimation((int)EAnimType::eWalk);

				mMoveSpeed += move * MOVE_SPEED;
				if (mSt < mMaxSt)
				{
					mSt++;
				}
			}
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == (int)EState::eIdle)
		{
			ChangeAnimation((int)EAnimType::eIdle);

			if (mSt < mMaxSt)
			{
				mSt++;
			}
		}
	}
}

// 指定した位置まで移動する
bool CPlayer2::MoveTo(const CVector& targetPos, float speed)
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
	else if (remainDist <= 20.0f)
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
void CPlayer2::Update()
{
	if (mHp <= 0)
	{
		ChangeState((int)EState::eDeath);
	}

	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
	// 待機状態
	case (int)EState::eIdle:		UpdateIdle();		break;
	// 仰け反り
	case (int)EState::eHit:			UpdateHit();		break;
	// 死亡処理
	case (int)EState::eDeath:		UpdateDeath();		break;
	// 妖力を注いでいる
	case (int)EState::eChanneling:	UpdateChanneling();	break;
	// 追従状態
	case(int)EState::eTracking:		UpdateTracking();	break;
	}

	// このプレイヤーが操作中であれば、
	if (IsOperate())
	{
		// 待機中は、移動処理を行う
		if (mState == (int)EState::eIdle)
		{
			UpdateMove();
		}


		// 重力
		mMoveSpeedY -= GRAVITY;

		// 移動
		CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);
		Position(Position() + moveSpeed);

		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));

		CVector p = Position();
		float distance = CVector::Distance(mLastPos, p);

		if (distance >= MAX_DISTANCE)
		{
			SetTrail();
		};
	}
	else
	{
		ChangeState((int)EState::eTracking);
	}

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
	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);
	CDebugPrint::Print("FPS:%f\n", Times::FPS());
#endif

	// 地面についているか
	mIsGrounded = false;

	// 体力ゲージの更新
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	// スタミナゲージの更新処理
	mpStGauge->SetMaxPoint(mMaxSt);
	mpStGauge->SetCurPoint(mSt);

#if _DEBUG
	// その場で復活
	if (mState == (int)EState::eDeath)
	{
		if (CInput::PushKey('R'))
		{
			mHp = 100;
			mState = (int)EState::eIdle;
		}
	}
#endif // _DEBUG

}

// ステータスを整数にして取得する
int CPlayer2::GetState()
{
	return static_cast<int>(mState);
}

// 攻撃中か
bool CPlayer2::IsAttacking() const
{
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
		ChangeState((int)EState::eHit);

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
	CPlayerBase::Collision(self, other, hit);
}

// 操作中かどうか設定
void CPlayer2::SetOperate(bool operate)
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