#include "CAdventurer.h"
#include "CCamera.h"
#include "Maths.h"
#include "CInput.h"
#include "CSword.h"
#include "CSlash.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CPlayerHpUI.h"
#include "CElementSlotUI2.h"
#include "CElementManager.h"
#include "ItemData.h"
#include "CInventory.h"
#include "CNavNode.h"
#include "CResultScene.h"

// プレイヤーのインスタンス
CAdventurer* CAdventurer::spInstance = nullptr;

// アニメーションのパス
#define ANIM_PATH "Character\\Player\\anim\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.75f	// 移動速度
#define RUN_SPEED 1.2f		// 走行中の速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define MAX_HP 100	// 体力の最大値

#define ATTACK_START_FRAME 26.0f	// 斬り攻撃の開始フレーム
#define ATTACK_END_FRAME 50.0f		// 斬り攻撃の終了フレーム

#define ATTACK2_START_FRAME 31.0f	// 斬り攻撃の開始フレーム
#define ATTACK2_EFFECT_START_FRAME 36.0f	// 斬撃の開始フレーム
#define ATTACK2_END_FRAME 40.0f		// 斬り攻撃の終了フレーム

// 斬り攻撃の剣のオフセット座標
#define ATTACK_SWORD_OFFSET_POS CVector(0.0f, 7.2f, 3.5f)
// 斬り攻撃の剣のオフセット向き
#define ATTACK_SWORD_OFFSET_ROT CVector(-20.0f, 0.0f, -7.0f)

// HPのUIの座標
#define HP_GAUGE_UI_POS CVector2(50.0f,600.0f)


// プレイヤーのアニメーションデータのテーブル
const CAdventurer::AnimData CAdventurer::ANIM_DATA[] =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// 待機
	{ ANIM_PATH"walk.x",		true,	66.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"run.x",			true,	43.0f,	1.0f	},	// 走行
	{ ANIM_PATH"attack.x",		false,	92.0f,	1.0f	},	// 斬り攻撃
	{ ANIM_PATH"attack2.x",		false,	79.0f,	1.0f	},	// 回転切り
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// 仰け反り

};

// インスタンスのポインタの取得
CAdventurer* CAdventurer::Instance()
{
	return spInstance;
}

// コンストラクタ
CAdventurer::CAdventurer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mpSword(nullptr)
	, mEquipElementSlotIndex(0)
	, mElementType(ElementType::None)
	, mpNavNode(nullptr)
{
	mMaxHp = MAX_HP;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

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
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy , ETag::eItem, ETag::eInteractObject });
	mpBodyCol->SetCollisionLayers
	(
		{
			ELayer::eFloor, ELayer::eWall,ELayer::eCeil,ELayer::eMoveCrate,
			ELayer::eEnemy,ELayer::eAttackCol ,ELayer::eCrystal,ELayer::eInteractObj
		}
	);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	// プレイヤーの剣を作成
	mpSword = new CSword
	(
		this,
		ETag::ePlayer,
		{ ETag::eEnemy },	// 敵のタグが設定されたコライダーと衝突
		{ ELayer::eEnemy }	// 敵のレイヤーが設定されたコライダーと衝突
	);

	// 右手のフレームを取得し、
	// 剣にプレイヤーの右手の行列をアタッチ
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpSword->SetAttachMtx(&frame->CombinedMatrix());
	mpSword->Position(ATTACK_SWORD_OFFSET_POS);
	mpSword->Rotation(ATTACK_SWORD_OFFSET_ROT);

	// 属性スロットのUI
	mpElementEquipment = new CElementSlotUI2();

	// プレイヤーのHPのUI
	mpHpGauge = new CPlayerHpUI(MAX_HP);
	mpHpGauge->SetPos(HP_GAUGE_UI_POS);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), CNavNode::ENodeType::ePlayer, true);
	mpNavNode->SetColor(CColor::white);

}

// デストラクタ
CAdventurer::~CAdventurer()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);

	// 剣が存在したら、
	if (mpSword != nullptr)
	{
		// 持ち主を解除してから、削除
		mpSword->SetOwner(nullptr);
		mpSword->Kill();
	}

	spInstance = nullptr;
}

// 攻撃中か
bool CAdventurer::IsAttacking() const
{
	// 斬り攻撃中
	if (mState == EState::eAttack) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CAdventurer::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	// 斬り攻撃中であれば、剣のコライダーをオンにする
	if (mState == EState::eAttack || mState == EState::eAttack2)
	{
		mpSword->SetEnableCol(true);
	}
	
}

// 攻撃終了
void CAdventurer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpSword->SetEnableCol(false);
}

// ダメージを受ける
void CAdventurer::TakeDamage(int damage, CObjectBase* causer)
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

//// 装備したスロット番号のアイテムを装備する
//void CAdventurer::EquipElement(int slotIndex)
//{
//	const CrystalData* data = CElementManager::Instance()->GetCurrentElement();
//
//	if (!data) return;
//
//	// 既に設定しているスロット番号と一致したら処理しない
//	if (slotIndex == mEquipElementSlotIndex)
//	{
//		// 装備しているスロット番号を記憶しておく
//		mEquipElementSlotIndex = slotIndex;
//
//		// 装備したアイテムをUIに設定
//		mpElementEquipment->SetElement(slotIndex, data);
//	}
//
//	// データが存在し
//	if (data)
//	{
//		// 記憶している属性と異なる場合、
//		if (mElementType != data->type)
//		{
//			// 装備している属性を記憶しておく
//			mElementType = data->type;
//
//			// 装備したアイテムをUIに設定
//			mpElementEquipment->SetElement(slotIndex, data);
//		}
//	}
//}

// 経路探索用のノード取得
CNavNode* CAdventurer::GetNavNode() const
{
	return mpNavNode;
}

// オブジェクト削除を伝える
void CAdventurer::DeleteObject(CObjectBase* obj)
{
	// 剣が先に削除されたら、剣のポインタを初期化
	if (mpSword == obj)
	{
		mpSword = nullptr;
	}
}

// キーの入力情報から移動ベクトルを求める
CVector CAdventurer::CalcMoveVec() const
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

// 待機状態
void CAdventurer::UpdateIdle()
{
	// 接地していれば、
	if (mIsGrounded)
	{
		// 左クリックで斬撃攻撃へ移行
		if (CInput::PushKey(VK_LBUTTON))
		{
			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack);
		}
		// 右クリックで属性を付与した斬撃
		else if (CInput::PushKey(VK_RBUTTON))
		{
			CElementManager* element = CElementManager::Instance();

			if(element->IsUseElementEnergy())
			{
				ChangeState(EState::eAttack2);
				mMoveSpeed = CVector::zero;

				element->UseElement();
				//mElementType = element->GetCurrentElement();
			}
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
	}
}

// 斬り攻撃
void CAdventurer::UpdateAttack()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃アニメーションを開始
		ChangeAnimation(EAnimType::eAttack, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= ATTACK_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始
			AttackStart();

			mStateStep++;
		}
		break;
	case 2:
		if (GetAnimationFrame() >= ATTACK_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mStateStep++;
		}
		break;
	case 3:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

// 回転切り
void CAdventurer::UpdateAttack2()
{
	switch (mStateStep)
	{
	case 0:
		// 攻撃アニメーション2を開始
		ChangeAnimation(EAnimType::eAttack2, true);
		//// 斬撃SEの再生済みフラグを初期化
		//mIsPlayedSlashSE = false;
		//// 斬撃エフェクトの生成済みフラグを初期化
		//mIsSpawnedSlashEffect = false;

		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= ATTACK2_START_FRAME)
		{
			// 斬撃SEを再生
			//mpSlashSE->Play();
			
			// 攻撃開始
			AttackStart();

			mStateStep++;
		}
		break;

	case 2:
		if (GetAnimationFrame() >= ATTACK2_EFFECT_START_FRAME)
		{
			// 斬撃SEを再生
			//mpSlashSE->Play();

			// 斬撃生成
			CSlash* slash = new CSlash
			(
				this,
				Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 1.0f,
				VectorZ(),
				100.0f,
				1000.0f
			);

			const CrystalData* data = CElementManager::Instance()->GetCurrentElement();

			if (data->type == ElementType::Fire)
			{
				// 赤
				slash->SetColor(CColor(1.0f, 0.0f, 0.0f, 1.0f));
			}
			else if (data->type == ElementType::Water)
			{
				// 青
				slash->SetColor(CColor::blue);
			}
			else if (data->type == ElementType::Thunder)
			{
				// 紫
				slash->SetColor(CColor(1.0f, 0.0f, 1.0f, 1.0f));
			}
			else
			{
				// 白
				slash->SetColor(CColor::white);
			}
			//slash->RotateAxis(VectorZ(), 45.0f);	// 斜めにする

			mStateStep++;
		}

			break;
	case 3:
		if (GetAnimationFrame() >= ATTACK2_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mStateStep++;
		}
		break;
	case 4:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}

}

// ジャンプ開始
void CAdventurer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CAdventurer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CAdventurer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

// 仰け反り
void CAdventurer::UpdateHit()
{
	switch (mStateStep)
	{
	case 0:
		// 仰け反りアニメーションを開始
		ChangeAnimation(EAnimType::eHit, true);
		mStateStep++;
		break;
	case 1:
		// 仰け反りアニメーションが終了したら
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

// 移動の更新処理
void CAdventurer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	bool isDash = false;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		// スペースキーを押していたら
		if (CInput::Key(VK_SHIFT))
		{
			// ダッシュする
			isDash = true;
		}
		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			if (isDash)
			{
				// 走るアニメーション
				ChangeAnimation(EAnimType::eRun); 
			}
			else
			{
				// 歩くアニメーション
				ChangeAnimation(EAnimType::eWalk);
			}
		}

		// 走行中かどうかで速度を変更
		mMoveSpeed += move * (isDash ? RUN_SPEED : MOVE_SPEED);
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

// アニメーション切り替え
void CAdventurer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
	CXCharacter::SetAnimationSpeed(data.speed);
}

// 状態を切り替え
void CAdventurer::ChangeState(EState state)
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

// 衝突処理
void CAdventurer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体のコライダーの衝突判定
	if (self == mpBodyCol)
	{
		// フィールドとの衝突
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
		// 天井と衝突した場合
		else if (other->Layer() == ELayer::eCeil)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			// 押し戻す方向が下方向(マイナス)であれば、天井と衝突した
			if (adjust.Y() < 0.0f)
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				Position(Position() + adjust * hit.weight);
			}
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
		// 木箱と衝突した場合
		else if (other->Layer() == ELayer::eMoveCrate)
		{
			// 自身と相手の座標
			CVector selfPos = Position();
			CVector otherPos = other->Owner()->Position();

			//　自身の座標から相手の座標までのベクトルを求める
			CVector vec = otherPos - selfPos;
			vec.Y(0.0f);
			// 正規化して方向ベクトル化
			CVector dir = vec.Normalized();

			// 木箱の法線格納用
			CVector normal = VectorZ();
			// Y軸に90度回転する回転行列を作成
			CMatrix rotMtx;
			rotMtx.RotateY(90.0f);

			// 4方向のベクトルと、相手までのベクトルの内積で角度を求め、
			// 一番近い方向から押し出しベクトルを求める
			float maxDot = -2.0f;
			CVector pushDir = CVector::zero;
			for (int i = 0; i < 4; i++)
			{
				// 内積で角度を求めて、一番近い(内積結果が大きい)方向ベクトルを取得
				float d = CVector::Dot(normal, dir);
				if (d > maxDot)
				{
					pushDir = normal;
					maxDot = d;
				}
				// 調べるベクトルをY軸に90度回転
				normal = rotMtx * normal;
			}

			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			// 求めた4方向で一番近いベクトル方向に押し戻す
			float length = CVector::Dot(pushDir, adjust * hit.weight);
			adjust = pushDir * length;
			Position(Position() + adjust);
		}
	}
	// 剣のコライダーが衝突した
	else if (self == mpSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
}

// 更新
void CAdventurer::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:			UpdateIdle();		break;
		// 斬り攻撃
	case EState::eAttack:		UpdateAttack();		break;
		// 回転切り
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

	// 経路探索用のノードが存在すれば、座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// マウスホイールの回転量を取得
	int mouseWheel = CInput::GetDeltaMouseWheel();
	// 属性スロットを変更
	if (mouseWheel > 0)
	{
		CElementManager::Instance()->SelectNext();
	}
	else if(mouseWheel < 0)
	{
		CElementManager::Instance()->SelectPrev();
	}

	// 「E」キーで
	if (CInput::PushKey('E'))
	{
		// 仮で炎属性を追加
		CElementManager::Instance()->AddElementEnergy(ElementType::Fire);
		// 回復薬を追加
		CInventory::Instance()->AddItem(ItemId::HealingPotion, 1);

	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// 「Q」キーを押したら、
	if (CInput::PushKey('Q'))
	{
		// 仮で水属性を追加
		CElementManager::Instance()->AddElementEnergy(ElementType::Water);
		// 鍵を追加
		CInventory::Instance()->AddItem(ItemId::Key, 1);
	}
	

	// 武器の行列を更新
	mpSword->UpdateMtx();
	
#if _DEBUG
	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);
	CDebugPrint::Print("FPS:%f\n", Times::FPS());
#endif

	mIsGrounded = false;

	// 体力ゲージの更新
	mpHpGauge->SetMaxPoint(mMaxHp);
	mpHpGauge->SetCurPoint(mHp);

}

// 後更新
void CAdventurer::LateUpdate()
{
	CXCharacter::LateUpdate();
}

// 描画
void CAdventurer::Render()
{
	CXCharacter::Render();
}
