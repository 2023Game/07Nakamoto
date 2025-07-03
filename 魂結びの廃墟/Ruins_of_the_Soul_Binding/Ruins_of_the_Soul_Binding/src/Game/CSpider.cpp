#include "CSpider.h"
#include "CColliderSphere.h"
#include "CPlayer2.h"
#include "CCat.h"
#include "CNavNode.h"
#include "CNavManager.h"

#include "CDebugFieldOfView.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Enemy\\Warrok\\anim\\"

#define FOV_ANGLE			 45.0f	// 視野範囲の角度
#define FOV_LENGTH			100.0f	// 視野範囲の距離
#define EYE_HEIGHT			 10.0f	// 視点の高さ

#define ATTACK_COL_RADIUS 10.0f	// 攻撃コライダーの半径
#define ATTACK_COL_POS CVector(0.0f, 5.0f, 8.0f)	// 攻撃コライダーの位置

#define ATTACK_POWER 20		// 攻撃力

#define SEARCH_RADIUS	 10.0f		// 壊せるオブジェクトを探知する範囲の半径

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",							true,	60.0f,	1.0f	},	// 待機		
	//{ ANIM_PATH"spider_walk.x",		true,	44.0f,	1.0f	},	// 歩行
	//{ ANIM_PATH"spider_run.x",		true,	27.0f,	1.0f	},	// 走行
	//{ ANIM_PATH"spider_attack.x",	false,	81.0f,	1.0f	},	// 攻撃

};

// コンストラクタ
CSpider::CSpider(std::vector<CVector> patrolPoints)
	: mIsBattle(false)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mpAttack1Col(nullptr)
	, mFovAngle(FOV_ANGLE)
	, mFovLength(FOV_LENGTH)
	, mpDebugFov(nullptr)
	, mNextPatrolIndex(-1)	// -1の時に一番近いポイントに移動
	, mNextMoveIndex(0)
{
	CPlayer2* target = CPlayer2::Instance();
	mTargets.push_back(target);

	CCat* cat = CCat::Instance();
	mTargets.push_back(cat);

	// 敵を初期化
	InitEnemy("Spider", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	mpBodyCol = new CColliderSphere
	(
		this, ELayer::eEnemy,
		3.0f
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
		{ ELayer::eFloor,
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
		this, ELayer::eInteractSearch,
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

}

// デストラクタ
CSpider::~CSpider()
{
}

// 攻撃中か
bool CSpider::IsAttacking() const
{
	return false;
}

// 攻撃開始
void CSpider::AttackStart()
{
}

// 攻撃終了
void CSpider::AttackEnd()
{
}

// ダメージを受ける
void CSpider::TakeDamage(int damage, CObjectBase* causer)
{
}

// 死亡
void CSpider::Death()
{
}

// 衝突処理
void CSpider::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
}

// オブジェクト削除を伝える関数
void CSpider::DeleteObject(CObjectBase* obj)
{
}

// 更新
void CSpider::Update()
{
}

// 描画
void CSpider::Render()
{
	CEnemy::Render();
}

// 状態切り替え
void CSpider::ChangeState(int state)
{
}

// 戦闘相手の方へ向く
void CSpider::LookAtBattleTarget(bool immediate)
{
}

// プレイヤーが視野範囲内に入ったかどうか
bool CSpider::IsFoundTarget(CObjectBase* target) const
{
	return false;
}

// 現在位置からプレイヤーが見えているかどうか
bool CSpider::IsLookTarget(CObjectBase* target) const
{
	return false;
}

// プレイヤーを攻撃できるかどうか
bool CSpider::CanAttackPlayer() const
{
	return false;
}

// 指定した位置まで移動する
bool CSpider::MoveTo(const CVector& targetPos, float speed)
{
	return false;
}

// 次に巡回するポイントを変更する
void CSpider::ChangePatrolPoint()
{
}

// 待機状態の更新処理
void CSpider::UpdateIdle()
{
}

// 状態の文字列を取得
std::string CSpider::GetStateStr(EState state) const
{
	return std::string();
}

// 状態の色を取得
CColor CSpider::GetStateColor(EState state) const
{
	return CColor();
}

// 一番近くにある壊せるオブジェクトを取得
CInteractObject* CSpider::GetNearBreakObj() const
{
	return nullptr;
}
