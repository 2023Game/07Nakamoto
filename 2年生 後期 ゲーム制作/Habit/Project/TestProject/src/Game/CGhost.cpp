#include "CGhost.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CNavNode.h"
#include "CNavManager.h"
//#include "CDebugFieldOfView.h"
#include "CPlayer2.h"
#include "CField.h"
#include "Primitive.h"
#include "CTrap.h"

#define BODY_HEIGHT 13.0f
#define BODY_RADIUS 5.0f

#define WALK_SPEED		50.0f	// 歩く速度
#define ROTATE_SPEED	6.0f	// 回転速度

#define FOV_ANGLE			 45.0f	// 視野範囲の角度
#define FOV_LENGTH			100.0f	// 視野範囲の距離
#define EYE_HEIGHT			 10.0f	// 視点の高さ

#define PATROL_INTERVAL		  3.0f	// 次の巡回ポイントに移動開始するまでの時間
#define PATROL_NEAR_DIST	 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME			  5.0f	// 待機状態の時間

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",		true,	59.0f,	1.0f	},	// 待機
};

// コンストラクタ
CGhost::CGhost(std::vector<CVector> patrolPoints)
	: mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	//, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1の時に一番近いポイントに移動
	, mNextMoveIndex(0)
	, mTrap(false)
{
	// 敵を初期化
	InitEnemy("Ghost", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	// 当たるコライダーの設定
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::ePlayer , ETag::eTrap});
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eTrap });

//#if _DEBUG
//	// 視野範囲のデバッグ表示クラスを作成
//	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);
//#endif

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// 巡回ポイントに経路探索用のノードを配置
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

}

// デストラクタ
CGhost::~CGhost()
{
	//// 視野範囲のデバッグ表示が存在したら、一緒に削除する
	//if (mpDebugFov != nullptr)
	//{
	//	mpDebugFov->SetOwner(nullptr);
	//	mpDebugFov->Kill();
	//}

	// 経路探索用のノードを破棄
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		SAFE_DELETE(mpNavNode);

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

// 攻撃中か
bool CGhost::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CGhost::AttackStart()
{
}

// 攻撃終了
void CGhost::AttackEnd()
{
}

// 衝突処理
void CGhost::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

	if (self == mpBodyCol)
	{
		if (other->Layer() == ELayer::eTrap)
		{
			mIsTrap = true;
		}
		else if (other->Layer() != ELayer::eTrap)
		{
			mIsTrap = false;
		}
	}
}

// オブジェクト削除を伝える関数
void CGhost::DeleteObject(CObjectBase* obj)
{
	//// 削除されたオブジェクトが視野範囲のデバッグ表示であれば
	//// 自身が削除された
	//if (obj == mpDebugFov)
	//{
	//	mpDebugFov = nullptr;
	//}
}

// 更新
void CGhost::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
		// 徘徊
	case EState::ePatrol:	UpdateePatrol(); break;
		// 追いかける
	case EState::eChase:	UpdateChase();	break;
	}

	// 敵のベースクラスの更新
	CEnemy::Update();

}

// 戦闘相手の方へ向く
void CGhost::LookAtBattleTarget(bool immediate)
{
}

// 状態切り替え
void CGhost::ChangeState(int state)
{
	// 攻撃中に他の状態へ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}

	// 状態切り替え
	CEnemy::ChangeState(state);
}

// 現在位置からプレイヤーが見えているかどうか
bool CGhost::IsFoundPlayer() const
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

	// ① 視野角度内か求める
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

	// ② 視野距離内か求める
	//プレイヤーまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// プレイヤーとの間に遮蔽物がないかチェックする
	if (!IsLookPlayer()) return false;

	//全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からプレイヤーが見えているかどうか
bool CGhost::IsLookPlayer() const
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


bool CGhost::MoveTo(const CVector& targetPos, float speed)
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
void CGhost::ChangePatrolPoint()
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

// 待機状態の更新処理
void CGhost::UpdateIdle()
{
	//// プレイヤーが視野範囲外に入ったら、追跡にする
	//if (IsFoundPlayer())
	//{
	//	ChangeState((int)EState::eChase);
	//	return;
	//}

	// 通常時の待機
	ChangeAnimation((int)EAnimType::eIdle);

	if (mElapsedTime < IDLE_TIME)
	{
		mElapsedTime += Times::DeltaTime();
	}
	else
	{
		// 待機時間が経過したら、巡回状態へ移行
		ChangeState((int)EState::ePatrol);
	}
}

// 徘徊状態の更新処理
void CGhost::UpdateePatrol()
{
	//if (IsFoundPlayer())
	//{
	//	ChangeState((int)EState::eChase);
	//	return;
	//}

	ChangeAnimation((int)EAnimType::eIdle);

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
		ChangeAnimation((int)EAnimType::eIdle);
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
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			ChangePatrolPoint();
			mStateStep = 1;
			mElapsedTime = 0.0f;

			if (!mIsTrap)
			{
				// 罠を生成
				CTrap* trap = new CTrap();
				trap->Position(Position() + CVector(0.0f, 1.0f, 0.0f));
			}
		}
		break;
	}
}

// 追いかける時の更新処理
void CGhost::UpdateChase()
{
	//mMoveSpeed = CVector::zero;

	//// 現在地と目的地を取得
	//CVector pos = Position();
	//CVector targetPos = mpBattleTarget->Position();
	//targetPos.Y(pos.Y());
	//// 現在地から目的地までのベクトルを求める
	//CVector vec = targetPos - pos;
	//// 攻撃範囲内であれば
	//float dist = vec.Length();
	//if (dist <= ATTACK_RANGE)
	//{
	//	// 攻撃状態へ移行
	//	ChangeState((int)EState::eAttack1);
	//}
	//// 攻撃範囲外
	//else
	//{
	//	// 歩行アニメーションを再生
	//	ChangeAnimation((int)EAnimType::eWalk);

	//	// 残り距離が移動距離より大きい場合は、移動距離分移動
	//	CVector dir = vec.Normalized();
	//	float moveDist = CHAISE_SPEED * Times::DeltaTime();
	//	if (dist > moveDist)
	//	{
	//		mMoveSpeed = dir * moveDist;
	//	}
	//	// 残り距離の方が小さい場合は、
	//	// 残り距離分移動して、待機状態へ移行
	//	else
	//	{
	//		mMoveSpeed = dir * dist;
	//		ChangeState((int)EState::eIdle);
	//	}
	//}

	//// 徐々に戦闘相手の方向へ向く
	//LookAtBattleTarget();

}
