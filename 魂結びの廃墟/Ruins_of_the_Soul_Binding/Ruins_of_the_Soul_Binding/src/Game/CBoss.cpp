#include "CBoss.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "Maths.h"
#include "CCactusNeedle.h"

#include "CPlayer2.h"
#include "CCat.h"
#include "CDebugFieldOfView.h"
#include "Primitive.h"
#include "CField.h"
#include "CDoor.h"
#include "CNavNode.h"
#include "CNavManager.h"
#include "CInteractObject.h"
#include "CDemonPowerManager.h"
#include "CSceneManager.h"
#include "CRoom.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Enemy\\Warrok\\anim\\"

#define BODY_HEIGHT 13.0f	// カプセルコライダーの上の高さ
#define BODY_WIDTH	5.0f	// カプセルコライダーの幅
#define BODY_RADIUS 4.5f	// カプセルコライダーの半径

#define WALK_SPEED		10.0f	// 歩く速度
#define RUN_SPEED		20.0f	// 走る速度
#define ROTATE_SPEED	6.0f	// 回転速度

#define LOOKAT_SPEED 90.0f	// 相手の方へ向く速さ
#define ATTACK_RANGE 15.0f	// 攻撃をする範囲
#define ATTACK_WAIT_TIME	  1.0f	// 攻撃終了時の待ち時間

#define ATTACK_COL_RADIUS 10.0f	// 攻撃コライダーの半径
#define ATTACK_COL_POS CVector(0.0f, 5.0f, 8.0f)	// 攻撃コライダーの位置

#define ATTACK_POWER 20		// 攻撃力
#define ATTACK_START_FRAME 39.0f	// 攻撃判定開始フレーム
#define ATTACK_END_FRAME 46.0f		// 攻撃判定終了フレーム

#define FOV_ANGLE			 50.0f	// 視野範囲の角度
#define FOV_LENGTH			200.0f	// 視野範囲の距離
#define FOV_HEIGHT			 10.0f	// 視野範囲の高さ
#define EYE_HEIGHT			 10.0f	// 視点の高さ

#define ALERT_STOP_TIME		  2.0f	// 警戒状態へ遷移する静止時間
#define ALERT_FOV_ANGLE		 60.0f	// 警戒時の視野範囲の角度
#define ALERT_FOV_LENGTH	300.0f	// 警戒時の視野範囲の距離

#define PATROL_INTERVAL		  3.0f	// 次の巡回ポイントに移動開始するまでの時間
#define PATROL_NEAR_DIST	 10.0f	// 巡回開始時に選択される巡回ポイントの最短距離
#define IDLE_TIME			  5.0f	// 待機状態の時間

#define SEARCH_RADIUS	 10.0f		// 壊せるオブジェクトを探知する範囲の半径

#define UPDATE_LOST_POS_TIME 2.0f	// 見失った位置を更新する時間

// ボスのインスタンス
CBoss* CBoss::spInstance = nullptr;

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",							true,	60.0f,	1.0f	},	// 待機		
	{ ANIM_PATH"warrok_walk.x",		true,	44.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"warrok_run.x",		true,	27.0f,	1.0f	},	// 走行
	{ ANIM_PATH"warrok_attack.x",	false,	81.0f,	1.0f	},	// 攻撃
	{ ANIM_PATH"warrok_alert.x",	false,	150.0f,	0.5f	},	// 警戒
};

// インスタンスのポインタを取得
CBoss* CBoss::Instance()
{
	return spInstance;
}

// コンストラクタ
CBoss::CBoss(std::vector<CVector> patrolPoints)
	: mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTargetChara(nullptr)
	, mpBattleTargetObj(nullptr)
	, mpAttack1Col(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mNextPatrolIndex(-1)	// -1の時に一番近いポイントに移動
	, mNextMoveIndex(0)
	, mPower(ATTACK_POWER)
	, mDemonPower(0)
	, mStopElapsedTime(0.0f)
#if _DEBUG
	, mpDebugFov(nullptr)
#endif
{
	spInstance = this;

	CPlayer2* target = CPlayer2::Instance();
	mTargetCharas.push_back(target);

	CCat* cat = CCat::Instance();
	mTargetCharas.push_back(cat);

	// 敵を初期化
	InitEnemy("Warrok", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_WIDTH, 0.0f),
		CVector(0.0f, BODY_HEIGHT, 0.0f),
		BODY_RADIUS, true
	);

	// フィールドと、ターゲットの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags
	(
		{ 
			ETag::eField,
			ETag::eGimmick,
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject
		}
	);
	mpBodyCol->SetCollisionLayers
	(
		{	ELayer::eFloor,
			ELayer::eWall,
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eInteractObj,
			ELayer::eDoor
		}
	);

	// 攻撃コライダーを作成
	mpAttack1Col = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS
	);
	// ターゲットの本体コライダーとのみヒットするように設定
	mpAttack1Col->SetCollisionTags
	(
		{ 
			ETag::ePlayer,
			ETag::eCat,
			ETag::eInteractObject,
		}
	);
	mpAttack1Col->SetCollisionLayers
	(
		{
			ELayer::ePlayer,
			ELayer::eCat,
			ELayer::eDoor,
		}
	);
	// 攻撃コライダーの座標を設定
	mpAttack1Col->Position(ATTACK_COL_POS);
	// 攻撃コライダーを最初はオフにしておく
	mpAttack1Col->SetEnable(false);

	// 壊せるオブジェクトを探知するコライダーを作成
	mpSearchCol = new CColliderSphere
	(
		this, ELayer::eBreakableSearch,
		SEARCH_RADIUS
	);
	// 壊せるオブジェクトのみ衝突するように設定
	mpSearchCol->SetCollisionTags({ ETag::eInteractObject });
	mpSearchCol->SetCollisionLayers({ ELayer::eDoor });

#if _DEBUG
	// 視野範囲のデバッグ表示クラスを作成
	mpDebugFov = new CDebugFieldOfView(this, mFovAngle, mFovLength);

#endif

	// 経路探索用のノードを作成
	mpNavNode = new CNavNode(Position(), true);
	mpNavNode->SetColor(CColor::blue);

	// ターゲットを見失った位置のノードを作成
	mpLostPlayerNode = new CNavNode(CVector::zero, true);
	mpLostPlayerNode->SetEnable(false);

	// 巡回ポイントに経路探索用のノードを配置
	for (CVector point : patrolPoints)
	{
		CNavNode* node = new CNavNode(point, true);
		mPatrolPoints.push_back(node);
	}

	// 妖力の源の数を取得
	mDemonPower = CDemonPowerManager::Instance()->GetDemonPower();
	//mMaxDemonPower = CDemonPowerManager::Instance()->GetDemonPower();
	//mDemonPower = mMaxDemonPower;

	// 頭の行列を取得
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_Head");
	mpHeadMtx = &frame->CombinedMatrix();

}

// デストラクタ
CBoss::~CBoss()
{
	// 体のコライダーを削除
	SAFE_DELETE(mpBodyCol);
	// 攻撃コライダーを削除
	SAFE_DELETE(mpAttack1Col);
	// 壊せるオブジェクトを探知するコライダーを削除
	SAFE_DELETE(mpSearchCol);

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
		mpNavNode->Kill();
		mpLostPlayerNode->Kill();

		// 巡回ノードに配置したノードも全て削除
		auto itr = mPatrolPoints.begin();
		auto end = mPatrolPoints.end();
		while (itr != end)
		{
			CNavNode* del = *itr;
			itr = mPatrolPoints.erase(itr);
			del->Kill();
		}
	}
}

// オブジェクト削除を伝える関数
void CBoss::DeleteObject(CObjectBase* obj)
{
	// 削除されたオブジェクトが視野範囲のデバッグ表示であれば
	// 自身が削除された
	if (obj == mpDebugFov)
	{
		mpDebugFov = nullptr;
	}
}

// 攻撃中か
bool CBoss::IsAttacking() const
{
	// パンチ攻撃
	if (mState == (int)EState::eAttack) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CBoss::AttackStart()
{
	// ベースクラスの攻撃開始処理を呼び出し
	CEnemy::AttackStart();

	// パンチ攻撃中であれば、パンチ攻撃のコライダーをオンにする
	if (mState == (int)EState::eAttack)
	{
		mpAttack1Col->SetEnable(true);
	}
}

// 攻撃終了
void CBoss::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemy::AttackEnd();

	// 攻撃コライダーをオフ
	mpAttack1Col->SetEnable(false);
}

// 妖力の源の減少
void CBoss::PowerDown()
{
	mDemonPower--;
}

// ダメージを受ける
void CBoss::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		// ChangeState((int)EState::eHit);

		// ダメージを与えた相手がキャラクターかどうか判定
		// TODO：飛び道具だった場合は、その持ち主を判定
		CCharaBase* chara = dynamic_cast<CCharaBase*>(causer);
		if (chara != nullptr)
		{
			// 攻撃を加えた相手を戦闘相手に設定
			mpBattleTargetChara = chara;

			// 攻撃を加えた相手の方向へ向く
			LookAtBattleTarget(true);

			// 戦闘状態へ切り替え
			mIsBattle = true;
		}

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 死亡
void CBoss::Death()
{
}

// 衝突処理
void CBoss::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

	// 攻撃コライダーがヒットした
	if (self == mpAttack1Col)
	{
		// ヒットしたのがキャラクターかつ、
		// まだ攻撃がヒットしていないキャラクターであれば
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr && !IsAttackHitObj(chara))
		{
			// ダメージを与える
			chara->TakeDamage(mPower, this);
			// 攻撃ヒット済みリストに登録
			AddAttackHitObj(chara);
		}

		CInteractObject* obj = dynamic_cast<CInteractObject*>(other->Owner());
		if (obj != nullptr && !IsAttackHitObj(obj))
		{
			// ダメージを与える
			obj->TakeDamage(mPower, this);
			// 攻撃ヒット済みリストに登録
			AddAttackHitObj(obj);
		}
	}
	// オブジェクトサーチ用の場合
	else if (self == mpSearchCol)
	{
		CObjectBase* obj = other->Owner();
		if (obj != nullptr)
		{
			// 壊せるオブジェクトの削除フラグが立っていないかつ、
			// 壊れている状態でなければ、
			if (!obj->IsKill() && !obj->IsDeath())
			{
				// 衝突した壊せるオブジェクトをリストに追加
				mNearBreakObjs.push_back(obj);
			}
		}
	}
}

// 戦闘相手の方へ向く
void CBoss::LookAtBattleTarget(bool immediate)
{
	CObjectBase* battleTarget = GetBattleTarget();
	// 戦闘相手がいなければ、処理しない
	if (battleTarget == nullptr) return;

	// 戦闘相手までの方向ベクトルを求める
	CVector targetPos = battleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// すぐに戦闘相手の方向へ向く
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// 徐々に戦闘相手の方向へ向く
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// 頭の正面方向ベクトルを取得
CVector CBoss::GetHeadForwardVec() const
{
	if (mpHeadMtx == nullptr) return VectorZ();
	
	CMatrix m;
	m.RotateX(-90.0f);
	m = m * (*mpHeadMtx);
	CVector vec = m.VectorZ();
	vec.Y(0.0f);

	CVector start = m.Position();
	CVector end = start + m.VectorZ() * 100.0f;

	// 確認用
	//Primitive::DrawLine(start, end, CColor::red, 2.0f);

	return vec.Normalized();
}

// 状態切り替え
void CBoss::ChangeState(int state)
{
	// 攻撃中に他の状態へ移行する場合は
	// 攻撃終了処理を呼び出す
	if (mState != state && IsAttacking())
	{
		AttackEnd();
	}
	// 状態切り替え
	CEnemy::ChangeState(state);
	mStopElapsedTime = 0.0f;
}

// 現在の戦闘相手を取得
CObjectBase* CBoss::GetBattleTarget() const
{
	// 壊すオブジェクトを優先して返す
	if (mpBattleTargetObj != nullptr)return mpBattleTargetObj;
	// 壊すオブジェクトが無ければ、戦闘相手のキャラクターを返す
	return mpBattleTargetChara;
}

// 現在位置からターゲットが見えているかどうか
bool CBoss::IsFoundTarget(CObjectBase* target) const
{
	// ターゲット座標を取得
	CVector targetPos = target->Position();
	// 自分自身の座標を取得
	CVector pos = Position();
	// 自身からターゲットまでのベクトルを求める
	CVector  vec = targetPos - pos;

	vec.Y(0.0f);	//ターゲットとの高さの差を考慮しない

	// ① 視野角度内か求める
	// ベクトルを正規化して長さを1にする
	CVector dir = vec.Normalized();
	// 自身の頭の正面方向ベクトルを取得
	CVector forward = GetHeadForwardVec();
	// ターゲットまでのベクトルと
	// 自身の正面方向ベクトルの内積を求めて角度を出す
	float dot = CVector::Dot(dir, forward);
	// 視野角度のラジアンを求める
	float angleR = Math::DegreeToRadian(mFovAngle);
	// 求めた内積と視野角度で、視野範囲内か判断する
	if (dot < cosf(angleR))	return false;

	// ② 視野距離内か求める
	//ターゲットまでの距離と視野距離で、視野範囲内か判断する
	float dist = vec.Length();
	if (dist > mFovLength)	return false;

	// ターゲットとの間に遮蔽物がないかチェックする
	if (!IsLookTarget(target)) return false;

	//全ての条件をクリアしたので、視野範囲内である
	return true;
}

// 現在位置からターゲットが見えているかどうか
bool CBoss::IsLookTarget(CObjectBase* target) const
{
	CVector targetPos = target->Position();
	CVector selfPos = Position();
	// ターゲットの高さで視野範囲内か判定
	float diffY = abs(targetPos.Y() - selfPos.Y());
	if (diffY >= FOV_HEIGHT) return false;

	// フィールドが存在しない場合は、遮蔽物がないので見える
	CField* field = CField::Instance();
	if (field == nullptr) return true;

	CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
	// ターゲットの座標を取得
	targetPos += offsetPos;
	// 自分自身の座標を取得
	selfPos += offsetPos;

	CHitInfo hit;
	//フィールドとレイ判定を行い、遮蔽物が存在した場合は、ターゲットが見えない
	if (field->CollisionRay(selfPos, targetPos, &hit)) return false;	

	// ターゲットとの間に遮蔽物がないので、ターゲットが見えている
	return true;

}

// ターゲットに攻撃できるかどうか
bool CBoss::CanAttackBattleTarget() const
{
	CObjectBase* battleTarget = GetBattleTarget();
	// ターゲットがいない場合は、攻撃できない
	if ((battleTarget == nullptr)) return false;

	// ターゲットまでの距離が攻撃範囲外であれば、攻撃できない
	CVector targetPos = battleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);	//高さを考慮しない

	float dist = vec.Length();
	if (dist > ATTACK_RANGE) return false;

	// すべての条件をクリアしたので、攻撃できる
	return true;
}

// 壊せるオブジェクトを攻撃するか確認
bool CBoss::CheckAttackBreakObj()
{
	// 戦闘相手（キャラクター）が存在しなければ、壊せるオブジェクトを攻撃しない
	if (mpBattleTargetChara == nullptr) return false;

	// 近くに壊せるオブジェクトがなければ、壊せるオブジェクトを攻撃しない
	CObjectBase* obj = GetNearBreakObj();
	if (obj == nullptr) return false;

	// 壊せるオブジェクトを攻撃対象にして、
	// 攻撃状態へ移行
	mpBattleTargetObj = obj;
	ChangeState((int)EState::eAttack);

	// 攻撃状態へ移行したので、trueを返す
	return true;
}

// キャラクターを攻撃するか確認
bool CBoss::CheckAttackChara()
{
	for (CCharaBase* target : mTargetCharas)
	{
		// ターゲットが死亡していたら、追跡対象としない
		if (target->IsDeath()) continue;
		// ターゲットが視野範囲内に入ったら、追跡にする
		if (IsFoundTarget(target))
		{
			// 戦闘相手（キャラクター）を設定
			mpBattleTargetChara = target;
			ChangeState((int)EState::eChase);
			return true;
		}
	}

	return false;
}

// 戦闘相手の死亡確認をして、戦闘相手がまだ存在するか確認
CObjectBase* CBoss::ChackDeathBattleTargets()
{
	// 戦闘相手（オブジェクト）を優先で死亡確認を行う
	if (mpBattleTargetObj != nullptr)
	{
		// 死亡していなければ、オブジェクトを返す
		if (!mpBattleTargetObj->IsDeath()) return mpBattleTargetObj;

		mpBattleTargetObj = nullptr;

	}
	// 戦闘相手（キャラクター）の死亡確認
	if (mpBattleTargetChara != nullptr)
	{
		// 死亡していなければ、キャラクターを返す
		if (!mpBattleTargetChara->IsDeath()) return mpBattleTargetChara;
		mpBattleTargetChara = nullptr;
	}
	
	// 戦闘相手がどちらも存在しなかったので、nullptrを返す
	return nullptr;
}

// 指定した位置まで移動する
bool CBoss::MoveTo(const CVector& targetPos, float speed)
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
	// 残りの距離がキャラクターの半径より短い場合
	else if (remainDist <= BODY_RADIUS)
	{
		// そこで移動を終了とする
		return true;
	}

	// 残りの距離が移動距離より長い場合は、
	// 移動距離分目的地へ移動する
	pos += moveDir * moveDist;
	Position(pos);

	// 目的地には到着しなかった
	return false;
}

// 次に巡回するポイントを変更する
bool CBoss::ChangePatrolPoint()
{
	// 自身の経路探索ノードが更新中の場合は処理しない
	if (mpNavNode->IsUpdating()) return false;
	// 巡回ポイントが設定されていない場合は処理しない
	int size = mPatrolPoints.size();
	if (size == 0) return false;

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

	return mNextPatrolIndex >= 0;
}

// 巡回ルートを更新する
bool CBoss::UpdatePatrolRouto()
{
	// 巡回ポイントの経路探索ノードの位置を設定しなおすことで、
	// 各ノードへの接続情報を更新
	for (CNavNode* node : mPatrolPoints)
	{
		node->SetPos(node->GetPos());
	}

	if (!(0 <= mNextPatrolIndex && mNextPatrolIndex < mPatrolPoints.size())) return false;

	CNavManager* navMgr = CNavManager::Instance();
	if(navMgr == nullptr) return false;

	// 自身のノードが更新中ならば、経路探索は行わない
	if (mpNavNode->IsUpdating()) return false;
	// 巡回ポイントが更新中ならば、経路探索を行う
	CNavNode* patrolPoint = mPatrolPoints[mNextPatrolIndex];
	if (patrolPoint->IsUpdating()) return false;
	
	// 巡回ポイントまでの最短経路を求める
	if (navMgr->Navigate(mpNavNode, patrolPoint, mMoveRoute))
	{
		// 次の目的地のインデックスを設定
		mNextMoveIndex = 1;
	}
}

// 待機状態の更新処理
void CBoss::UpdateIdle()
{
	// 攻撃するキャラクターが見つかった場合は、この処理を実行しない
	if (CheckAttackChara())
	{
		return;
	}

	// 待機アニメーションを再生
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

// 巡回中の更新処理
void CBoss::UpdatePatrol()
{
	// 攻撃するキャラクターが見つかった場合は、この処理を実行しない
	if (CheckAttackChara())
	{
		return;
	}

	ChangeAnimation((int)EAnimType::eWalk);

	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
	// ステップ0 : 巡回開始の巡回ポイントを求める
	case 0:
		mNextPatrolIndex = -1;
		if (ChangePatrolPoint())
		{
			mStateStep++;
		}
		
		break;
	// ステップ1 : 巡回ポイントまでの移動経路探索
	case 1:
		if(UpdatePatrolRouto())
		{
			mStateStep++;
		}
		break;
	// ステップ2 : 巡回ポイントまで移動
	case 2:
	{
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
	// ステップ3 : 移動後の待機
	case 3:
		ChangeAnimation((int)EAnimType::eIdle);
		if (mElapsedTime < PATROL_INTERVAL)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			if (ChangePatrolPoint())
			{
				mStateStep = 1;
				mElapsedTime = 0.0f;
			}
		}
		break;
	}
}

// 追いかける時の更新処理
void CBoss::UpdateChase()
{
	// 戦闘相手が存在しなかった場合は、
	CObjectBase* battleTarget = ChackDeathBattleTargets();
	if (battleTarget == nullptr)
	{
		// ターゲットを解除し、待機状態へ戻す
		ChangeState((int)EState::eIdle);
		return;
	}

	// 一定時間静止すると、警戒状態へ移行
	if (mStopElapsedTime >= ALERT_STOP_TIME)
	{
		// 移動が終われば警戒状態へ移行
		ChangeState((int)EState::eAlert);
		return;
	}

	// ターゲットの座標へ向けて移動する
	CVector targetPos = battleTarget->Position();

	// ターゲットが見えなくなったら、見失った状態にする
	if (!IsLookTarget(battleTarget))
	{
		// 見失った位置にノードを配置
		mpLostPlayerNode->SetPos(targetPos);
		mpLostPlayerNode->SetEnable(true);
		ChangeState((int)EState::eLost);
		return;
	}

	// ターゲットに攻撃できるならば、
	if (CanAttackBattleTarget())
	{
		// ターゲットの高さの差分が視野範囲内ならば
		float diffY = abs(battleTarget->Position().Y() - Position().Y());
		if (diffY < FOV_HEIGHT)
		{
			// 攻撃状態へ移行
			ChangeState((int)EState::eAttack);
		}
		// 視野範囲外であれば、警戒状態へ移行
		else
		{
			ChangeState((int)EState::eAlert);
		}
		return;
	}
 
	// 近くの壊せるオブジェクトを壊すのであれば追跡状態を抜ける
	if (CheckAttackBreakObj())
	{
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation((int)EAnimType::eRun);

	// 経路探索管理クラスが存在すれば、
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// 自身のノードとターゲットのノードが更新中でなければ、
		// 自身のノードからターゲットのノードまでの最短経路を求める
		CNavNode* targetNode = battleTarget->GetNavNode();
		if (!mpNavNode->IsUpdating()
			&& targetNode != nullptr && !targetNode->IsUpdating())
		{
			if (navMgr->Navigate(mpNavNode, targetNode, mMoveRoute))
			{
				// 自身のノードからターゲットのノードまで繋がっていたら、
				// 移動する位置を次のノードの位置に設定
				targetPos = mMoveRoute[1]->GetPos();
			}
		}
	}

	// 移動処理
	if (MoveTo(targetPos, RUN_SPEED))
	{
	}
}

// ターゲットを見失った時の更新処理
void CBoss::UpdateLost()
{
	CNavManager* navMgr = CNavManager::Instance();

	// 戦闘相手が存在しなかった場合は、
	CObjectBase* battleTarget = ChackDeathBattleTargets();
	if (battleTarget == nullptr)
	{
		// ターゲットを解除し、待機状態へ戻す
		ChangeState((int)EState::eIdle);
		return;
	}

	// ターゲットが見えたら、追跡状態へ移行
	if (IsLookTarget(battleTarget))
	{
		ChangeState((int)EState::eChase);
		return;
	}

	// 近くの壊せるオブジェクトを壊すのであれば、見失った状態を抜ける
	if (CheckAttackBreakObj())
	{
		return;
	}

	// 一定時間静止すると、警戒状態へ移行
	if (mStopElapsedTime >= ALERT_STOP_TIME)
	{
		// 移動が終われば警戒状態へ移行
		ChangeState((int)EState::eAlert);
		return;
	}

	// 走るアニメーションを再生
	ChangeAnimation((int)EAnimType::eRun);

	switch (mStateStep)
	{
			// ステップ0：見失った位置までの最短経路を求める
		case 0:
			mLostElapsedTime = 0.0f;
			mStateStep++;
			break;
		case 1:
			// 経路探索用のノード座標を更新
			mpLostPlayerNode->SetPos(battleTarget->Position());

			// 自身のノードと見失った位置のノードが更新中でなければ
			if (!mpNavNode->IsUpdating() && !mpLostPlayerNode->IsUpdating())
			{
				if (!navMgr->Navigate(mpNavNode, mpLostPlayerNode, mMoveRoute))
				{
					// 経路がつながっていなければ、待機状態へ戻す
					ChangeState((int)EState::eIdle);
					mpLostPlayerNode->SetEnable(false);
				}
				else
				{
					// 見失った位置まで経路が繋がっていたら、次のステップへ
					// 次の移動先を１番近い場所に設定
					mNextMoveIndex = 1;
				}
			}
		case 2:
			// 次の移動先のインデックス値が不正値でなければ
			if (0 <= mNextMoveIndex && mNextMoveIndex < mMoveRoute.size())
			{
				// ターゲットを見失った位置まで移動
				if (MoveTo(mMoveRoute[mNextMoveIndex]->GetPos(), RUN_SPEED))
				{
					mNextMoveIndex++;
					if (mNextMoveIndex >= mMoveRoute.size())
					{
						// 移動が終われば警戒状態へ移行
						ChangeState((int)EState::eAlert);
						mpLostPlayerNode->SetEnable(false);
					}
				}

				if (mLostElapsedTime < UPDATE_LOST_POS_TIME)
				{
					// 見失った時間を加算
					mLostElapsedTime += Times::DeltaTime();
					if (mLostElapsedTime >= UPDATE_LOST_POS_TIME)
					{
						mStateStep++;
					}
				}
			}
			break;
	}
}

// 警戒している時の処理
void CBoss::UpdateAlert()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
	// ステップ１：警戒アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAlert);
		mStateStep++;
		break;
	// ステップ２：警戒開始
	case 1:
		// 攻撃できるキャラクターが存在した場合は、そこで終了
		if (CheckAttackChara())
		{
			return;
		}

		// アニメーションが終わったら
		if (IsAnimationFinished())
		{
			// 待機k状態へ移行
			ChangeState((int)EState::eIdle());
		}
		break;
	}
}

// パンチ攻撃時の更新処理
void CBoss::UpdateAttack1()
{
	// ステップごとに処理を切り替える
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack, true);
		mStateStep++;
		break;
		// ステップ1：攻撃開始
	case 1:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK_START_FRAME)
		{
			// 攻撃開始処理を呼び出す
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2：攻撃終了
	case 2:
		if (GetAnimationFrame() >= ATTACK_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();
			mStateStep++;
		}
		break;
		// ステップ3：攻撃アニメーション終了待ち
	case 3:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			mStateStep++;
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
			// 戦闘相手の死亡チェック
			CObjectBase* batteleTarget = ChackDeathBattleTargets();
			// 戦闘相手が存在しなかった場合
			if (batteleTarget == nullptr)
			{
				// 待機状態へ移行
				ChangeState((int)EState::eIdle);
			}
			// 戦闘相手が存在した場合
			else
			{
				// 戦闘相手が視野範囲内にいた場合は、追跡状態へ移行
				if (IsLookTarget(batteleTarget))
				{
					ChangeState((int)EState::eChase);
				}
				// 戦闘相手が視野範囲内にいなかった場合、見失った状態へ移行
				else
				{
					mpLostPlayerNode->SetPos(batteleTarget->Position());
					mpLostPlayerNode->SetEnable(true);
					ChangeState((int)EState::eLost);
				}
			}
		}
		break;
	}
}

// 針攻撃時の更新処理
void CBoss::UpdateAttack2()
{
}

// 仰け反り状態の更新処理
void CBoss::UpdateHit()
{
}

// 死亡状態の更新処理
void CBoss::UpdateDeath()
{
	// シーンをクリアシーンに変更
	CSceneManager::Instance()->LoadScene(EScene::eClear);
}

// 更新
void CBoss::Update()
{
	CVector currPos = Position();
	float moveDist = (currPos - mLastPos).Length();
	if (moveDist <= 0.1f)
	{
		mStopElapsedTime += Times::DeltaTime();
	}
	else
	{
		mStopElapsedTime = 0.0f;
	}
	mLastPos = Position();

#if _DEBUG
	CDebugPrint::Print("moveDist:%f\n", moveDist);
	CDebugPrint::Print("StopTime:%f\n", mStopElapsedTime);
#endif

	// 妖力の源の数を取得
	int demonPower = CDemonPowerManager::Instance()->GetDemonPower();
	// 妖力の源が残り3つの時、
	if (demonPower == 3)
	{
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}
	// 妖力の源が残り3つの時、
	else if (demonPower == 2)
	{
		mPower = ATTACK_POWER + ATTACK_POWER;
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}
	// 妖力の源が残り3つの時、
	else if (demonPower == 1)
	{
		//CDebugPrint::Print("DemonPower:%d\n", demonPower);
	}
	else if (demonPower == 0)
	{
		ChangeState((int)EState::eDeath);
	}

	// 警戒時は視野範囲が広がる
	if (mState == (int)EState::eAlert)
	{
		mFovAngle = ALERT_FOV_ANGLE;
		mFovLength = ALERT_FOV_LENGTH;
	}
	// 通常時の視野範囲を設定
	else
	{
		mFovAngle = FOV_ANGLE;
		mFovLength = FOV_LENGTH;
	}

#if _DEBUG
	mpDebugFov->Set(mFovAngle, mFovLength);
#endif

	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
	// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
	// 巡回状態
	case EState::ePatrol:	UpdatePatrol();	break;
	// 追いかける
	case EState::eChase:	UpdateChase();	break;
	// 見失った状態
	case EState::eLost:		UpdateLost();	break;
	// パンチ攻撃
	case EState::eAttack:	UpdateAttack1(); break;
	// 警戒状態
	case EState::eAlert:	UpdateAlert();	break;
	// 死亡状態
	case EState::eDeath:	UpdateDeath();	break;
	}

	// 敵のベースクラスの更新
	CEnemy::Update();

	// 経路探索用のノード座標を更新
	if (mpNavNode != nullptr)
	{
		mpNavNode->SetPos(Position());
	}

	// 壊せるオブジェクトのリストをクリア
	mNearBreakObjs.clear();

#if _DEBUG
	// 現在の状態に合わせて視野範囲の色を変更
	mpDebugFov->SetColor(GetStateColor((EState)mState));

	CDebugPrint::Print("状態 : %s\n", GetStateStr((EState)mState).c_str());
	CDebugPrint::Print("部屋(ボス)：%s\n", mpRoom != nullptr ? mpRoom->GetName().c_str() : "なし");


#endif
}

// 描画
void CBoss::Render()
{
	CEnemy::Render();

#if _DEBUG

	// 視野範囲のデバッグ表示に頭の向きを反映
	CVector vec = GetHeadForwardVec();
	mHeadForwardMtx = CMatrix::LookRotation(vec, CVector::up);
	mHeadForwardMtx.Position(Position());
	mpDebugFov->SetMatrix(&mHeadForwardMtx);

	// 巡回中であれば、
	if (mState == (int)EState::ePatrol)
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
	if (mState == (int)EState::eLost)
	{
		//ターゲットを見失った位置にデバッグ表示
		float rad = 2.0f;
		Primitive::DrawBox
		(
			mpLostPlayerNode->GetPos() + CVector(0.0f, rad, 0.0f),
			CVector::one * rad,
			CColor::blue
		);
	}

	CField* field = CField::Instance();

	CObjectBase* battleTarget = GetBattleTarget();

	if (battleTarget != nullptr && field != nullptr)
	{
		CVector offsetPos = CVector(0.0f, EYE_HEIGHT, 0.0f);
		CVector targetPos = battleTarget->Position() + offsetPos;
		CVector selfPos = Position() + offsetPos;

		//ターゲットとの間に遮蔽物が存在する場合
		CHitInfo hit;
		if (field->CollisionRay(selfPos, targetPos, &hit))
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
			// ターゲットの位置まで緑線を描画
			Primitive::DrawLine
			(
				selfPos, targetPos,
				CColor::green,
				2.0f
			);
		}
	}
#endif
}

// 状態の文字列を取得
std::string CBoss::GetStateStr(EState state) const
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
CColor CBoss::GetStateColor(EState state) const
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

// 一番近くにある壊せるオブジェクトを取得
CObjectBase* CBoss::GetNearBreakObj() const
{
	// キャラクターを追いかけている状態でなければ、壊せるオブジェクトを壊さない
	if (mpBattleTargetChara == nullptr) return nullptr;
	// 既に壊せるオブジェクトを攻撃中であれば、新しく壊せるオブジェクトを設定しない
	if (mpBattleTargetObj != nullptr) return nullptr;

	// 一番近くの壊せるオブジェクトのポインタ格納用
	CObjectBase* nearObj = nullptr;
	float nearDist = 0.0f;	// 現在一番近くにある壊せるオブジェクトまでの距離
	CVector pos = Position();	// 敵自身の座標を取得
	// 探知範囲内の壊せるオブジェクトを順番に調べる
	for (CObjectBase* obj : mNearBreakObjs)
	{
		// 壊せるオブジェクトがドアかどうか確認
		CDoorBase* door = dynamic_cast<CDoorBase*>(obj);
		if (door != nullptr)
		{
			// プレイヤーが入っている部屋を取得
			CRoom* playerRoom = mpBattleTargetChara->GetRoom();
			// プレイヤーと自身が同じ部屋にいる場合は、ドアを壊さない
			if (playerRoom == mpRoom) continue;

			// プレイヤーが自身がいる部屋のドアでなければ、スルー
			CRoom* doorRoom = door->GetRoom();
			if (playerRoom != doorRoom && mpRoom != doorRoom) continue;
		}
		// オブジェクトの座標を取得
		CVector objPos = obj->Position();
		// ターゲットからオブジェクトまでのベクトルを求める
		CVector vec = objPos - pos;
		vec.Y(0.0f);	// オブジェクトとの高さの差を考慮しない

		// ベクトルを正規化して長さを1にする
		CVector dir = vec.Normalized();
		// 自身の正面方向ベクトルを取得
		CVector forward = VectorZ();
		// ターゲットとまでのベクトルと
		// 自身の正面方向ベクトルの内積を求めて角度を出す
		float dot = CVector::Dot(dir, forward);
		// 視野角度のラジアンを求める
		float angleR = Math::DegreeToRadian(mFovAngle);
		// 求めた内積と視野角度で、視野範囲内か判断する
		if (dot < cosf(angleR)) continue;

		float dist = (obj->Position() - pos).LengthSqr();
		// 一番最初の壊せるオブジェクトか、
		// 求めた距離が現在の一番近いオブジェクトよりも近い場合は、
		if (nearObj == nullptr || dist < nearDist)
		{
			// 一番近いオブジェクトを更新
			nearObj = obj;
			nearDist = dist;
		}
	}
	return nearObj;
}