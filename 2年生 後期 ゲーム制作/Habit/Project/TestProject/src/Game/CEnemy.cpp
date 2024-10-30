#include "CEnemy.h"
#include "CEffect.h"
#include "CInput.h"
#include "CCollisionManager.h"
#include "CDebugFieldOfView.h"
#include "CPlayer.h"
#include "Maths.h"
#include "Primitive.h"

#define FOV_ANGLE			45.0f	// 視野範囲の角度
#define FOV_LENGTH			100.0f	// 視野範囲の距離
#define WALK_SPEED			10.0f	// 歩いているときの速度
#define RUN_SPEED			20.0f	// 走っていいるときの速度
#define ROTATE_SPEED		6.0f	// 回転速度
#define ATTACK_RANGE		20.0f	// 攻撃範囲

#define ATTACK_MOVE_DIST	20.0f	// 攻撃時の移動距離
#define ATTACK_MOVE_STAT	16.0f	// 攻撃時の移動開始フレーム
#define ATTACK_MOVE_END		46.0f	// 攻撃時の移動終了フレーム
#define ATTACK_WAIT_TIME	1.0f	// 攻撃終了時の待ち時間

// プレイヤーのアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",														true,	122.0f	},	// 待機
	{ "Character\\Enemy\\mutant\\anim\\mutant_walk.x",			true,	44.0f	},	// 歩行
	{ "Character\\Enemy\\mutant\\anim\\mutant_run.x",			true,	27.0f	},	// 走行
	{ "Character\\Enemy\\mutant\\anim\\mutant_jump.x",			true,	96.0f	},	// ジャンプ
	{ "Character\\Enemy\\mutant\\anim\\mutant_jump_attack.x",	true,	112.0f	},	// ジャンプ攻撃
	{ "Character\\Enemy\\mutant\\anim\\mutant_attack.x",		true,	81.0f	},	// 攻撃


};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	//, mLostPlayerPos(CVector::zero)
	//, mAttackStartPos(CVector::zero)
	//, mAttackEndPos(CVector::zero)
	//, mNextPatrolIndex(-1)
{
	//モデルデータの取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

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

	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

}

// デストラクタ
CEnemy::~CEnemy()
{
	// 視野範囲のデバッグ表示が存在したら、一緒に削除する
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}
}

void CEnemy::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが視野範囲のデバッグ表示であれば
	// 自身が削除されたと
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// 更新処理
void CEnemy::Update()
{
	if (CInput::Key('U'))
	{
		ChangeState(EState::eChase);
		//ChangeAnimation(EAnimType::eWalk);
	}
	else if (CInput::Key('L'))
	{
		ChangeState(EState::ePatrol);
		//ChangeAnimation(EAnimType::eRun);
	}

	switch (mState)
	{
	case EState::eIdle:		UpdateIdle();		break;
	case EState::ePatrol:	UpdatePatrol();		break;
	case EState::eChase:	UpdateChase();		break;
	case EState::eLost:		UpdateLost();		break;
	case EState::eAttack:	UpdateAttack();		break;
	}

	CXCharacter::Update();

	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("状態 : %s\n", GetStateStr(mState).c_str());
}

// 描画処理
void CEnemy::Render()
{
	CXCharacter::Render();

	// 見失った状態であれば、
	if (mState == EState::eLost)
	{
		//プレイヤーを見失った位置にデバッグ表示
		float rad = 2.0f;
		CMatrix m;
		m.Translate(mLostPlayerPos + CVector(0.0f, rad, 0.0f));
		Primitive::DrawWireSphere(m, rad, CColor::blue);
	}
}

// アニメーションの切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength);
}

// 状態を切り替え
void CEnemy::ChangeState(EState state)
{
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;

}

bool CEnemy::IsFoundPlayer() const
{
	// プレイヤーが存在しない場合は、視野範囲外とする
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return false;

	// プレイヤー座標を取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からプレイヤーまでのベクトルを求める
	CVector  vec = playerPos - pos;
	vec.Y(0.0f);	//プレイヤーとの高さの差を考慮しない

	// ベクトルを正規化して長さを1にする
	CVector dir = vec.Normalized();
	// 自身の正面方向ベクトルを取得
	CVector forward = VectorZ();
	// プレイヤーとまでのベクトルと
	// 自身の正面方向ベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と視野角度で、視野範囲内か判断する
	if (dot < cosf(angleR))	return false;

	//プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// TODO:プレイヤーとの間に遮蔽物がないかチェックする



	//全ての条件をクリアしたので、視野範囲内である
	return true;
}

// プレイヤーを攻撃できるかどうか
//bool CEnemy::CanAttackPlayer() const
//{
//	// プレイヤーがいない場合は、攻撃できない
//	CPlayer* player = CPlayer::Instance();
//	if ((player == nullptr)) return false;
//
//	// プレイヤーまで
//	CVector playerPos = player->Position();
//	CVector vec = playerPos - Position();
//	vec.Y(0.0f);
//	float dist = vec.Length();
//	if (dist > ATTACK_RANGE) return false;
//
//	return true;
//}

////指定した位置まで移動する
bool CEnemy::MoveTo(const CVector& targetPos, float speed)
{
	// 目的地までのベクトルを求める
	CVector pos = Position();
	CVector vec = targetPos - pos;
	vec.Y(0.0f);
	// 移動方向ベクトルを求める
	CVector moveDir = vec.Normalized();

	// 徐々に移動方向へ移動
	CVector forward = CVector::Slerp
	(
		VectorZ(),	// 現在の正面方向
		moveDir,	// 移動方向
		ROTATE_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));

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

	// 残りの距離が移動距離より長い場合は、
	// 移動距離分目的地へ移動する
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}


// 待機状態時の更新処理
void CEnemy::UpdateIdle()
{
	// プレイヤーが視野範囲外に入ったら、追跡にする
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
}

// 巡回中の更新処理
void CEnemy::UpdatePatrol()
{
}

// 追跡時の更新処理
void CEnemy::UpdateChase()
{
	// プレイヤーが視野範囲外に出たら、見失った状態にする
	if (!IsFoundPlayer())
	{
		ChangeState(EState::eLost);
		return;
	}

//	// プレイヤーに攻撃できるならば、攻撃状態へ移行
//	if (CanAttackPlayer())
//	{
//		ChangeState(EState::eAttack);
//		return;
//	}

	// 走るアニメーションを再生
	ChangeAnimation(EAnimType::eRun);

	// プレイヤーの座標へ向けて移動する
	CPlayer* player = CPlayer::Instance();
	CVector playerPos = player->Position();
	mLostPlayerPos = playerPos;	// プレイヤーを最後に見た座標を更新
	if (MoveTo(playerPos, RUN_SPEED))
	{
	}
}

// プレイヤーを見失った時の更新処理
void CEnemy::UpdateLost()
{
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation(EAnimType::eRun);
	// プレイヤーを見失った位置まで移動
	if (MoveTo(mLostPlayerPos, RUN_SPEED))
	{
		// 移動が終われば待機状態へ移行
		ChangeState(EState::eIdle);
	}
}

// 攻撃時の更新処理
void CEnemy::UpdateAttack()
{
	//// ステップ後のにしょりうを切り替える
	//switch (mStateStep)
	//{
	//	// ステップ0 : 攻撃アニメーションを再生
	//	case 0:
	//		ChangeAnimation(EAnimType::eAttack);
	//		mStateStep++;
	//		break;
	//	// ステップ1 : 
	//	case 1:
	//	{
	//		// 攻撃アニメーションが移動開始フレームを超えた
	//		float frame = GetAnimationFrame();
	//		if (frame >= ATTACK_MOVE_STAT)
	//		{
	//			// 線形補間で移動開始位置から移動終了位置まで移動する
	//			float moveFrame = ATTACK_MOVE_END - ATTACK_MOVE_STAT;
	//			float percent = (frame - ATTACK_MOVE_STAT) / moveFrame;
	//			CVector pos = CVector::Lerp(mAttackStartPos, mAttackEndPos, percent);
	//			Position(pos);
	//		}
	//		// 移動終了フレームまで到達した
	//		else
	//		{

	//		}
	//		break;
	//	}
	//	case 2:
	//		break;
	//	// ステップ3 : 攻撃終了
	//	case 3:
	//		break;
	//}

	//ChangeAnimation(EAnimType::eAttack);
}

// 状態の文字列を取得
std::string CEnemy::GetStateStr(EState state) const
{
	switch (state)
	{
	case EState::eIdle:		return "待機";
	case EState::ePatrol:	return "巡回";
	case EState::eChase:	return "追跡";
	case EState::eLost:		return "見失う";
	case EState::eAttack:	return "攻撃";
	}
	return "";
}

// 状態の色を取得
CColor CEnemy::GetStateColor(EState state) const
{
	switch (state)
	{
	case EState::eIdle:		return CColor::white;
	case EState::ePatrol:	return CColor::green;
	case EState::eChase:	return CColor::red;
	case EState::eLost:		return CColor::yellow;
	case EState::eAttack:	return CColor::magenta;
	}
	return CColor::white;
}
