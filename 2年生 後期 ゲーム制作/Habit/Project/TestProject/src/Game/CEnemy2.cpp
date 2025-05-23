#include "CEnemy2.h"
#include "CInput.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CDebugFieldOfView.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "Primitive.h"
#include "CField.h"
#include "CNavNode.h"
#include "CNavManager.h"

#define FOV_ANGLE			 45.0f	// 視野範囲の角度
#define FOV_LENGTH			100.0f	// 視野範囲の距離
#define EYE_HEIGHT			 10.0f	// 視点の高さ
#define WALK_SPEED			 50.0f	// 歩いているときの速度
#define RUN_SPEED			 80.0f	// 走っていいるときの速度
#define ROTATE_SPEED		 6.0f	// 回転速度

#define ENEMY_HEIGHT_CCOL1	24.0f	// カプセルコライダーの上の高さ
#define ENEMY_HEIGHT_CCOL2	2.0f	// カプセルコライダーの下の高さ
#define ENEMY_WIDTH_CCOL	5.0f	// カプセルコライダーの幅

#define ATTACK_RANGE		 20.0f	// 攻撃範囲
#define ATTACK_WAIT_TIME	  1.0f	// 攻撃終了時の待ち時間

#define ATTACK_JUDGE_START	39.0f	// 攻撃判定開始フレーム
#define ATTACK_JUDGE_END	46.0f	// 攻撃判定終了フレーム
#define ATTACK_DAMAGE		50		// 攻撃のダメージ

#define PATROL_INTERVAL		  3.0f	// 次の巡回ポイントに移動開始するまでの時間
#define PATROL_NEAR_DIST	 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME			  5.0f	// 待機状態の時間

#define GAUGE_OFFSET_Y 15.0f

// エネミー2のアニメーションデータのテーブル
const CEnemy2::AnimData CEnemy2::ANIM_DATA[] =
{
	{ "",													true,	60.0f   },	// 待機		
	{ "Character\\Enemy\\warrok\\anim\\warrok_walk.x",		true,	44.0f	},	// 歩行
	{ "Character\\Enemy\\warrok\\anim\\warrok_run.x",		true,	27.0f	},	// 走行
	{ "Character\\Enemy\\warrok\\anim\\warrok_attack.x",	false,	81.0f	},	// 攻撃

};

// コンストラクタ
CEnemy2::CEnemy2(std::vector<CVector> patrolPoints)
	: CXCharacter(ETag::eEnemy, ETaskPriority::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1の時に一番近いポイントに移動
	, mNextMoveIndex(0)
	, mAttackHit(false)
{
	//モデルデータの取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy2");

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
		this, ELayer::eEnemy,
		CVector(0.0f, ENEMY_HEIGHT_CCOL2, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT_CCOL1, 0.0f),
		ENEMY_WIDTH_CCOL
	);
	mpColliderCapsule->SetCollisionTags({ ETag::eField, ETag::ePlayer });
	mpColliderCapsule->SetCollisionLayers
	(
		{	ELayer::eField,
			ELayer::eWall,
			ELayer::ePlayer,
			ELayer::eInteractObj}
	);
	
	// 攻撃用の球コライダ―を作成
	mpAttackCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		10.0f, true
	);
	mpAttackCollider->SetCollisionTags({ ETag::ePlayer });
	mpAttackCollider->SetCollisionLayers({ ELayer::ePlayer });
	mpAttackCollider->Position(0.0f, 5.0f, 10.0f);
	// 攻撃用コライダーを最初はオフにしておく
	mpAttackCollider->SetEnable(false);

	//腕にコライダーを付けるとき
	//// 攻撃用の球コライダ―を作成
	//mpAttackCollider = new CColliderSphere
	//(
	//	this, ELayer::eAttackCol,
	//	40.0f,true
	//);
	//mpAttackCollider->SetCollisionTags({ ETag::ePlayer });
	//mpAttackCollider->SetCollisionLayers({ ELayer::ePlayer });

	//// 左手のボーンの行列を取得
	//CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_LeftHand");
	//const CMatrix& mtx = frame->CombinedMatrix();
	//// 攻撃用のコライダーを左手の行列に設定
	//mpAttackCollider->SetAttachMtx(&mtx);
	//// 攻撃用コライダーを最初はオフにしておく
	//mpAttackCollider->SetEnable(false);

	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// プレイヤーを見失った位置のノードを作成
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);

	// 巡回ポイントに経路探索用のノードを配置
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}
}

// デストラクタ
CEnemy2::~CEnemy2()
{
	SAFE_DELETE(mpColliderCapsule);
	SAFE_DELETE(mpAttackCollider);

	// 視野範囲のデバッグ表示が存在したら、一緒に削除する
	if (mpDebugFov != nullptr)
	{
		mpDebugFov->SetOwner(nullptr);
		mpDebugFov->Kill();
	}

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);
		SAFE_DELETE(mpLostPlayerNode);

		// 巡回ノードに配置したノードも全て削除
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPoints.erase(itr);
			delete del;
		}
	}
}

void CEnemy2::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが視野範囲のデバッグ表示であれば
	// 自身が削除された
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// 更新処理
void CEnemy2::Update()
{
	switch (mState)
	{
	case EState::eIdle:		UpdateIdle();		break;
	case EState::ePatrol:	UpdatePatrol();		break;
	case EState::eChase:	UpdateChase();		break;
	case EState::eLost:		UpdateLost();		break;
	case EState::eAttack:	UpdateAttack();		break;
	}

	CXCharacter::Update();
	mpAttackCollider->Update();

	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor(mState));

	CDebugPrint::Print("状態 : %s\n", GetStateStr(mState).c_str());
}

// 描画処理
void CEnemy2::Render()
{
	CXCharacter::Render();

#if _DEBUG
	// 巡回中であれば、
	if (mState == EState::ePatrol)
	{
		float rad = 1.0f;
		int size = mPatrolPoints.size();
		for (int i = 0; i < size; i++)
		{
			CColor c = i == mNextPatrolIndex ? CColor::red : CColor::cyan;
			Primitive::DrawBox
			(
				mPatrolPoints[i]->GetPos() + CVector(0.0f, rad * 2.0f, 0.0f),
				CVector::one * rad,
				c
			);
		}
	}
	// 見失った状態であれば、
	if (mState == EState::eLost)
	{
		//プレイヤーを見失った位置にデバッグ表示
		float rad = 2.0f;
		Primitive::DrawBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f),
			CVector::one * rad,
			CColor::blue
		);
	}

	CPlayer2* player = CPlayer2::Instance();
	CField* field = CField::Instance();
	if (player != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector playerPos = player->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		//プレイヤーとの間に遮蔽物が存在する場合
		CHitInfo hit;
		if (field->CollisionRay(selfPos, playerPos, &hit))
		{
			// 衝突した位置まで赤線を描画
			Primitive::DrawLine
			(
				selfPos, hit.cross,
				CColor::red,
				2.0f
			);
		}
		// 遮蔽物が存在しなかった場合
		else
		{
			// プレイヤーの位置まで緑線を描画
			Primitive::DrawLine
			(
				selfPos, playerPos,
				CColor::green,
				2.0f
			);
		}
	}
#endif
}

// 衝突処理
void CEnemy2::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	//CEnemy::Collision(self, other, hit);

	// 攻撃コライダーがヒットした
	if (self == mpAttackCollider)
	{
		// ヒットしたのがキャラクターかつ、
		// まだ攻撃がヒットしていないキャラクターであれば
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// ダメージを与える
			chara->TakeDamage(ATTACK_DAMAGE, this);
			// 攻撃ヒット済みリストに登録
			AddAttackHitObj(chara);
		}
	}
}

// アニメーションの切り替え
void CEnemy2::ChangeAnimation(EAnimType type, bool restart)
{
	int index = (int)type;
	if (!(0 <= index && index < (int)EAnimType::Num)) return;
	const AnimData& data = ANIM_DATA[index];
	CXCharacter::ChangeAnimation(index, data.loop, data.framelength, restart);
}

// 状態を切り替え
void CEnemy2::ChangeState(EState state)
{	
	// 既に同じ状態であれば、処理しない
	if (state == mState) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// プレイヤーが視野範囲内に入ったかどうか
bool CEnemy2::IsFoundPlayer() const
{
	// プレイヤーが存在しない場合は、視野範囲外とする
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;

	// プレイヤーが死亡状態の場合も、範囲外とする
	if (player->GetState() == 8) return false;

	// プレイヤー座標を取得
	CVector playerPos = player->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からプレイヤーまでのベクトルを求める
	CVector  vec = playerPos - pos;
	vec.Y(0.0f);	//プレイヤーとの高さの差を考慮しない

	// �@ 視野角度内か求める
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

	// �A 視野距離内か求める
	//プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// プレイヤーとの間に遮蔽物がないかチェックする
	if (!IsLookPlayer()) return false;

	//全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からプレイヤーが見えているかどうか
bool CEnemy2::IsLookPlayer() const
{
	// プレイヤーが存在しない場合は、見えない
	CPlayer2* player = CPlayer2::Instance();
	if (player == nullptr) return false;
	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// プレイヤーの座標を取得
	CVector playerPos = player->Position() + offsetPos;
	// 自分自身の座標を取得
	CVector selfPos = Position() + offsetPos;

	CHitInfo hit;
	//フィールドとレイ判定を行い、遮蔽物が存在した場合は、プレイヤーが見えない
	if (field->CollisionRay(selfPos, playerPos, &hit)) return false;

	// プレイヤーとの間に遮蔽物がないので、プレイヤーが見えている
	return true;
}

// プレイヤーを攻撃できるかどうか
bool CEnemy2::CanAttackPlayer() const
{
	// プレイヤーがいない場合は、攻撃できない
	CPlayer2* player = CPlayer2::Instance();
	if ((player == nullptr)) return false;

	// プレイヤーまでの距離が攻撃範囲外であれば、攻撃できない
	CVector playerPos = player->Position();
	CVector vec = playerPos - Position();
	vec.Y(0.0f);	//高さを考慮しない

	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// すべての条件をクリアしたので、攻撃できる
	return true;
}

// 指定した位置まで移動する
bool CEnemy2::MoveTo(const CVector& targetPos, float speed)
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

// 次に巡回するポイントを変更する
void CEnemy2::ChangePatrolPoint()
{
	// 巡回ポイントが設定されていない場合は処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return;

	// 巡回開始時であれば、一番近い巡回ポイントを選択
	if (mNextPatrolIndex == -1)
	{
		int nearIndex = -1;		// 一番近い巡回ポイントの番号
		float nearDist = 0.0f;	// 一番近い巡回ポイントまでの距離
		// 全ての巡回ポイントの距離を調べ、一番近い巡回ポイントを探す
		for (int i = 0; i < size; i++)
		{
			CVector point = mPatrolPoints[i]->GetPos();
			CVector vec = point - Position();
			vec.Y(0.0f);
			float dist = vec.Length();
			// 巡回ポイントが近すぎる場合は、スルー
			if (dist < PATROL_NEAR_DIST) continue;
			// 一番最初の巡回ポイントもしくは、
			// 現在一番近い巡回ポイントよりさらに近い場合は、
			// 巡回ポイントの番号を置き換える
			if (nearIndex < 0 || dist < nearDist)
			{
				nearIndex = i;
				nearDist = dist;
			}
		}
		mNextPatrolIndex = nearIndex;
	}
	// 巡回中だった場合、次の巡回ポイントを指定する
	else
	{
		mNextPatrolIndex++;
		if (mNextPatrolIndex >= size) mNextPatrolIndex -= size;
	}

	if (mNextPatrolIndex >= 0)
	{
		CNavManager* navMgr = CNavManager::Instance();
		if (navMgr != nullptr)
		{
			// 経路探索用のノード座標を更新
			mpNavNode->SetPos(Position());

			// 巡回ポイントの経路探索ノードの位置を設定しなおすことで、
			// 各ノードへの接続情報を更新
			for (CNavNode* node : mPatrolPoints)
			{
				node->SetPos(node->GetPos());
			}
			// 巡回ポイントまでの最短経路を求める
			if (navMgr->Navigate(mpNavNode, mPatrolPoints[mNextPatrolIndex], mMoveRoute))
			{
				// 次の目的地のインデックスを設定
				mNextMoveIndex = 1;
			}
		}
	}
}

// 待機状態時の更新処理
void CEnemy2::UpdateIdle()
{
	// プレイヤーが視野範囲外に入ったら、追跡にする
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// 待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState(EState::ePatrol);
	}
}

// 巡回中の更新処理
void CEnemy2::UpdatePatrol()
{
	if (IsFoundPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	ChangeAnimation(EAnimType::eWalk);

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0 : 巡回会指示の巡回ポイントを求める
	case 0:
		mNextPatrolIndex = -1;
		ChangePatrolPoint();
		mStateStep++;
		break;
		// ステップ1 : 巡回ポイントまで移動
	case 1:
	{
		ChangeAnimation(EAnimType::eWalk);
		// 最短経路の次のノードまで移動
		CNavNode* moveNode = mMoveRoute[mNextMoveIndex];
		if (MoveTo(moveNode->GetPos(), WALK_SPEED))
		{
			// 移動が終われば、次のノードまで移動
			mNextMoveIndex++;
			// 最後のノード(目的地のノード)だった場合は、次のステップへ進める
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				mStateStep++;
			}
		}
		break;
	}
	// ステップ2 : 移動後の待機
	case 2:
		ChangeAnimation(EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;
		}
		break;
	}
}

// 追跡時の更新処理
void CEnemy2::UpdateChase()
{
	// プレイヤーの座標へ向けて移動する
	CPlayer2* player = CPlayer2::Instance();
	CVector targetPos = player->Position();

	// プレイヤーが見えなくなったら、見失った状態にする
	if (!IsLookPlayer())
	{
		// 見失った位置にノードを配置
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState(EState::eLost);
		return;
	}

	// プレイヤーに攻撃できるならば、攻撃状態へ移行
	if (CanAttackPlayer())
	{
		ChangeState(EState::eAttack);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation(EAnimType::eRun);

	// 経路探索管理クラスが存在すれば、
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 経路探索用のノード座標を更新
		mpNavNode->SetPos(Position());

		// 自身のノードからプレイヤーのノードまでの最短経路を求める
		CNavNode* playerNode = player->GetNavNode();
		if (navMgr->Navigate(mpNavNode, playerNode, mMoveRoute))
		{
			// 自身のノードからプレイヤーのノードまで繋がっていたら、
			// 移動する位置を次のノードの位置に設定
			targetPos = mMoveRoute[1]->GetPos();
		}
	}
	// 移動処理
	if (MoveTo(targetPos, RUN_SPEED))
	{
	}
}

// プレイヤーを見失った時の更新処理
void CEnemy2::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr == nullptr)
	{
		ChangeState(EState::eIdle);
		return;
	}
	// プレイヤーが見えたら、追跡状態へ移行
	if (IsLookPlayer())
	{
		ChangeState(EState::eChase);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation(EAnimType::eRun);

	switch (mStateStep)
	{
		// ステップ0：見失った位置までの最短経路を求める
	case 0:
		// 経路探索用のノード座標を更新
		mpNavNode->SetPos(Position());

		if (navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
		{
			// 見失った位置まで経路が繋がっていたら、次のステップへ
			mNextMoveIndex = 1;
			mStateStep++;
		}
		else
		{
			// 経路がつながっていなければ、待機状態へ戻す
			ChangeState(EState::eIdle);
			mpLostPlayerNode->SetEnable(false);
		}
		break;
	case 1:
		// プレイヤーを見失った位置まで移動
		if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
		{
			mNextMoveIndex++;
			if (mNextMoveIndex >= mMoveRoute.size())
			{
				// 移動が終われば待機状態へ移行
				ChangeState(EState::eIdle);
				mpLostPlayerNode->SetEnable(false);
			}
		}
		break;
	}
}

// 攻撃時の更新処理
void CEnemy2::UpdateAttack()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ０：攻撃アニメーションを再生
		case 0:
			// ベースクラスの攻撃開始処理を呼び出し
			CXCharacter::AttackStart();

			ChangeAnimation(EAnimType::eAttack, true);
			mStateStep++;
			break;
		// ステップ１: 攻撃用コライダーを有効にする
		case 1:
			if (GetAnimationFrame() >= ATTACK_JUDGE_START)
			{
				mpAttackCollider->SetEnable(true);
				mStateStep++;
			}
			break;
		// ステップ２： 攻撃用コライダーを無効にする
		case 2:
			if (GetAnimationFrame() >= ATTACK_JUDGE_END)
			{
				mpAttackCollider->SetEnable(false);
				mStateStep++;
			}
			break;
		// ステップ３ : 攻撃アニメーションの終了待ち
		case 3:
			if (IsAnimationFinished())
			{
				mStateStep++;
				mAttackHit = false;
			}
			break;
		// ステップ４ : 攻撃終了後の待ち時間
		case 4:
			if (mElapsedTime < ATTACK_WAIT_TIME)
			{
				mElapsedTime += Times::DeltaTime();
			}
			else
			{
				// 時間が経過したら、待機状態へ移行
				ChangeState(EState::eIdle);
			}
			break;
	}
}

// 状態の文字列を取得
std::string CEnemy2::GetStateStr(EState state) const
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
CColor CEnemy2::GetStateColor(EState state) const
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