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

// アニメーションのパス
#define ANIM_PATH "Character\\Cat\\anim\\"

#define BODY_HEIGHT 12.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 4.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define EYE_HEIGHT	7.0f	// 視点の高さ

#define TRAIL_SPEED 20.0f	// 追従時の速度
#define TRACKING_DIST 30.0f	// プレイヤーから離れたらついてくる距離

// カメラの回転速度
#define ROTATE_SPEED 1.5f

// 猫のインスタンス
CCat* CCat::spInstance = nullptr;

// 猫のアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
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
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->Rotate(90.0f, 0.0f, 0.0f);
	mpBodyCol->Position(0.0f, BODY_RADIUS, 8.0f);

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
			ELayer::eEnemy,
			ELayer::eAttackCol,
			ELayer::eDoor,
		}
	);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::white);

	// 追従時に障害物を避けるためのノードを作成
	mpTrackingNode = new CNavNode(CVector::zero, true);
	mpTrackingNode->SetEnable(false);
}

// デストラクタ
CCat::~CCat()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpTrackingNode);

		// 巡回ノードに配置したノードも全て削除
		auto itr = mTrackingPoints.begin();
		auto end = mTrackingPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mTrackingPoints.erase(itr);
			delete del;
		}
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

	}
}

// 追従
void CCat::UpdateTracking()
{
	CPlayer2 *player = CPlayer2::Instance();
	CVector playerPos = player->Position();		// プレイヤーまでのベクトル
	CVector playerVec = playerPos - Position();	// プレイヤーまでの距離

	// ベクトルの2乗を求める(処理不可が軽いので)
	float playerDist = playerVec.LengthSqr();
	// プレイヤーから一定距離離れると、ついていく座標を更新
	if (playerDist >= TRACKING_DIST * TRACKING_DIST)	// ベクトルの2乗と比較するのでTRACKING_DISTも2乗する
	{
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
		
		mpTrackingNode = new CNavNode(mFollowPos, true);
		mStateStep = 0;
	}

	if (!IsLookTarget(player))
	{
		mpTrackingNode->SetPos(player->Position());
		mpTrackingNode->SetEnable(true);
		ChangeState((int)EState::eLost);
		return;
	}

	switch (mStateStep)
	{
		// ステップ0：目標位置まで移動
		case 0:
			if (MoveTo(mFollowPos, TRAIL_SPEED))
			{
				mLookAtPos = playerPos;
				mStateStep++;
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

	// 操作フラグがtrueになったら、
	if (IsOperate())
	{
		// 待機状態にする
		ChangeState((int)EState::eIdle);
	}
}

// 見失った状態
void CCat::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState((int)EState::eIdle);
		return;
	}

	CPlayer2* player = CPlayer2::Instance();
	// ターゲットが見えたら、追跡状態へ移行
	if (IsLookTarget(player))
	{
		ChangeState((int)EState::eTracking);
		return;
	}

	switch (mStateStep)
	{
		// ステップ0：見失った位置までの最短経路を求める
	case 0:
		// 経路探索用のノード座標を更新
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpTrackingNode, mMoveRoute))
		{
			// 見失った位置まで経路が繋がっていたら、次のステップへ
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// 経路がつながっていなければ、追従状態へ戻す
			ChangeState((int)EState::eIdle);
			mpTrackingNode->SetEnable(false);
		}
		break;
	case 1:
		// ターゲットを見失った位置まで移動
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), TRAIL_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// 移動が終われば追従状態へ移行
				ChangeState((int)EState::eIdle);
				mpTrackingNode->SetEnable(false);
			}
		}
		break;
	}

	// 操作キャラになったら
	if (IsOperate())
	{
		ChangeState((int)EState::eIdle);
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
	// 見失った状態
	case (int)EState::eLost:		UpdateLost();	break;
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
	}
	else
	{
		mState == (int)EState::eTracking;
	}

	mMoveSpeedY -= GRAVITY;
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	// 猫を操作中だったら、
	if (mIsOperate)
	{
		// マウスの左右移動で、猫を左右に回転
		CVector2 delta = CInput::GetDeltaMousePos();
		Rotate(0.0f, delta.X() * ROTATE_SPEED * Times::DeltaTime(), 0.0f);
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
	CPlayerBase::Update();

	//CVector pos = Position();
	//CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	//CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	//CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	//CDebugPrint::Print("PlayerState:%d\n", mState);

	// 地面についているか
	mIsGrounded = false;

	//CDebugPrint::Print("FPS:%f\n", Times::FPS());

}

// 描画
void CCat::Render()
{
	// 猫を操作していたら、頭のない猫の体を描画
	if (mIsOperate)
	{

	}
	// 猫を操作してなけらば、全身を描画
	else
	{
		CPlayerBase::Render();
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
		//mpHpGauge->SetShow(true);
		//mpStGauge->SetShow(true);
	}
	// 自身が操作プレイヤーでない場合
	else
	{
		// 自身が操作する時のUIを非表示
		//mpHpGauge->SetShow(false);
		//mpStGauge->SetShow(false);
	}

}