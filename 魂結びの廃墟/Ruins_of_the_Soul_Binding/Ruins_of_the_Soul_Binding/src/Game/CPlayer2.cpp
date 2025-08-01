//プレイヤークラスのインクルード
#include "CPlayer2.h"
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
#include "CRoom.h"
#include "ItemData.h"
#include "CEquipmentUI.h"
#include "CInventory.h"
#include "CFireball.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Player2\\Rusk2\\anim\\"

#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define RUN_SPEED	1.0f	// 走る速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define MAX_HP 100	// 体力の最大値
#define MAX_ST 100	// スタミナの最大値
#define HP_GAUGE_UI_POS 100.0f, 40.0f
#define SP_GAUGE_UI_POS 110.0f, 70.0f

#define DEMON_POWER_DAMAGE	1	// 妖力の源へのダメージ

#define SEARCH_RADIUS	 10.0f		// 調べるオブジェクトを探知する範囲の半径
#define FOV_ANGLE		 60.0f		// 視野範囲の角度
#define FOV_LENGTH		 10.0f		// 視野範囲の距離

#define CHANNELING_TIME	0.5f		// 妖力を流し込んでダメージが入るまでの時間

#define MAX_DISTANCE 5.0f	// 追従する距離を更新する際の距離
#define MAX_HISTORY_SIZE 5	// 追従用のリストの制限数

#define RESERVED_CAPACITYE 7	// リストの初期容量

#define EQIOPMENT_UI_POS 1150.0f, 620.0f	// 装備中のアイテムのUI座標
#define EQIOPMENT_UI_ALPHA 0.8f				// 装備中のアイテムのUIのアルファ値
// スタミナ増減値(１秒間に増減する値）
#define STAMINA_DELTA 20.0f

#define ITEM_RECAST_TIME 5.0f	// アイテムの再使用できるまでの時間

// 火球の発射位置のオフセット座標
#define FIREBALL_OFFSET_POS CVector(0.0f, 5.0f, 10.0f)
#define FIREBALL_SPEED 100.0f	// 火球の速度
#define FIREBALL_DIST 200.0f	// 火球が移動できる距離

// プレイヤーのインスタンス
CPlayer2* CPlayer2::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const std::vector<CPlayerBase::AnimData> ANIM_DATA =
{
	{ "",						true,	301.0f,	1.0f	},	// 待機
	{ANIM_PATH"walk.x",			true,	31.0f,	0.5f	},	// 歩行
	{ANIM_PATH"run.x",			true,	22.0f,	0.5f	},	// 走行
	{ANIM_PATH"hit.x",			false,	37.0f,	1.0f	},	// ダメージを受けた時(仮)
	{ANIM_PATH"died.x",			false,	96.0f,	0.7f	},	// 死亡

	{ANIM_PATH"channeling_start.x",	false,	32.0f,	0.5f	},	// 妖力を流し込み開始
	{ANIM_PATH"channeling.x",		true,	3.0f,	0.3f	},	// 妖力を流し込み中
	{ANIM_PATH"channeling_end.x",	false,	39.0f,	0.5f	},	// 妖力を流し込み終了

};

// コンストラクタ
CPlayer2::CPlayer2()
	: CPlayerBase(ETag::ePlayer)
	, mMaxSt(MAX_ST)
	, mSt(mMaxSt)
	, mChannelingTime(0)
	, mTogether(true)
	, mEquipItemSlotIndex(-1)
	, mItemRecastTime(0.0f)
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

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::white);

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
			ELayer::eDoor,
			ELayer::eEnemy,
			ELayer::eDemon,
			ELayer::eAttackCol,
			ELayer::eSwitch,
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
	mpSearchCol->SetCollisionLayers({ ELayer::eInteractObj , ELayer::eDemon});

	mTrails.reserve(RESERVED_CAPACITYE);
	SetTrail();

	// HPゲージ作成
	mpHpGauge = new CHpGauge();
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	mpHpGauge->SetPos(HP_GAUGE_UI_POS);
	// スタミナゲージの作成
	mpStGauge = new CStGauge();
	mpStGauge->SetMaxPoint((int)mMaxSt);
	mpStGauge->SetCurPoint(ceilf(mSt));
	mpStGauge->SetPos(SP_GAUGE_UI_POS);

	mpEquipment = new CEquipmentUI();
	mpEquipment->SetPos(EQIOPMENT_UI_POS);
	mpEquipment->SetAlpha(EQIOPMENT_UI_ALPHA);
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

	//SAFE_DELETE(mpSearchCol);
	spInstance = nullptr;
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

// アイテムのクールタイム
void CPlayer2::ItemRecast()
{
	mItemRecastTime -= Times::DeltaTime();

	if (mItemRecastTime < 0.0f)
	{
		mItemRecastTime = 0.0f;
	}
}

// 火球を発射
void CPlayer2::ShotFireball()
{
	CVector pos = Position() + Rotation() * FIREBALL_OFFSET_POS;
	CVector forward = VectorZ();

	CFireball* fireball = new CFireball(FIREBALL_SPEED, FIREBALL_DIST);
	fireball->Position(pos);
}

// 待機
void CPlayer2::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// 近くの調べるオブジェクトの確認
		CheckNearInteractObj();

		// 何かアイテムを装備しているか
		if (mEquipItemSlotIndex >= 0)
		{
			if (mItemRecastTime <= 0.0f)
			{
				// 装備している状態で左クリックで、アイテムを使用
				if (CInput::PushKey(VK_LBUTTON))
				{
					CInventory::Instance()->UseItemSlot(mEquipItemSlotIndex);

					mItemRecastTime = ITEM_RECAST_TIME;
				}
			}
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
		CSceneManager::Instance()->LoadScene(EScene::eGameOver);
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
		if (CInput::Key('E') && IsOperate())
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

// 行動の状態を変更
void CPlayer2::ChangeAction()
{
	mTogether = !mTogether;
}

// 一緒に行動しているか
bool CPlayer2::GatActingTogether()
{
	return mTogether;
}

// 指定したアイテムを使用できるかどうか
bool CPlayer2::CanUseItem(const ItemData* item)
{
	switch (item->effectType)
	{
		// HP回復アイテムは、現在のHPが減っている時は使用可
	case ItemEffectType::RecoveryHP:
		return mHp < mMaxHp;

	case ItemEffectType::Throw:
		return true;

	}

	return false;
}

// アイテムの効果を使う
void CPlayer2::UseItem(const ItemData* item)
{
	switch (item->effectType)
	{
		// 回復アイテムの場合
	case ItemEffectType::RecoveryHP:
		mHp = mHp + item->recovery;

		if (mHp > mMaxHp)
		{
			mHp = mMaxHp;
		}
		break;
		// 投擲アイテムの場合
	case ItemEffectType::Throw:

		break;
	}
}

// 指定したスロット番号のアイテムを装備
void CPlayer2::EquipItem(int slotIndex)
{
	// 既に設定しているスロット番号と一致したら処理しない
	if (slotIndex == mEquipItemSlotIndex) return;

	// 装備しているスロット番号を記憶しておく
	mEquipItemSlotIndex = slotIndex;
	// 装備したアイテムをUIに設定
	const ItemData* itemData = CInventory::Instance()->GetItemSlotData(mEquipItemSlotIndex);
	mpEquipment->EquipItem(itemData);

}

// 装備しているアイテムスロットの番号を返す
int CPlayer2::GetEquipItemSlotIndex() const
{
	return mEquipItemSlotIndex;
}

// プレイヤーのバウンディングボックスを返す
const CBounds& CPlayer2::GetBounds() const
{
	return mpBodyCol->Bounds();
}

const CEquipmentUI& CPlayer2::GetEquipmentUI() const
{
	return *mpEquipment;
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

// 近くの調べるオブジェクトの確認処理
void CPlayer2::CheckNearInteractObj()
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
		CGameUI::Instance()->ShowInteractUI(obj);

		// 近くの調べられるオブジェクトが、妖力の源であれば、
		if (CDemonPower* dp = dynamic_cast<CDemonPower*>(obj))
		{
			// 妖力の源の近くで、
			// [E]キーを押したら、妖力を流し込む
			if (CInput::PushKey('E'))
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
		CGameUI::Instance()->HideInteractUI();
	}

	// 操作中で無ければ、
	if (!IsOperate())
	{
		// 調べるUIを非表示にする
		CGameUI::Instance()->HideInteractUI();
	}
}

// 移動の更新処理
void CPlayer2::UpdateMove()
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

				// 走るアニメーション
				ChangeAnimation((int)EAnimType::eRun);

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

		if (!mIsDash)
		{
			ChangeAnimation((int)EAnimType::eWalk);
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
	}

	// このプレイヤーが操作中であれば、
	if (IsOperate())
	{
		// 待機中は、移動処理を行う
		if (mState == (int)EState::eIdle)
		{
			UpdateMove();
		}
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

	// 「F」キーを押したら、火球を発射
	if (CInput::PushKey('F'))
	{
		ShotFireball();
	}

	// キャラクターの更新
	CPlayerBase::Update();

#if _DEBUG
	// 操作中なら
	if (IsOperate())
	{
		CVector pos = Position();
		//CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
		CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
		//CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
		CDebugPrint::Print("PlayerState:%d\n", mState);
		CDebugPrint::Print("FPS:%f\n", Times::FPS());
	}
	CDebugPrint::Print("部屋(人)：%s\n", mpRoom != nullptr ? mpRoom->GetName().c_str() : "なし");
	CDebugPrint::Print("Recast:%f\n", mItemRecastTime);

#endif

	// 地面についているか
	mIsGrounded = false;

	CVector p = Position();
	float distance = CVector::Distance(mLastPos, p);

	if (distance >= MAX_DISTANCE)
	{
		SetTrail();
	};

	if (mItemRecastTime > 0.0f)
	{
		ItemRecast();
	}


	// 体力ゲージの更新
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);
	// スタミナゲージの更新処理
	mpStGauge->SetMaxPoint((int)mMaxSt);
	mpStGauge->SetCurPoint(ceilf(mSt));
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

// 追従する位置を設定
void CPlayer2::SetTrail()
{
	CVector pos = Position();
	mLastPos = pos;

	mTrails.push_back(pos);

	if (mTrails.size() > MAX_HISTORY_SIZE)
	{
		mTrails.erase(mTrails.begin());
	}
}

const std::vector<CVector>& CPlayer2::GetTrail() const
{
	return mTrails;
}
