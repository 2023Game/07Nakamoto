#pragma once
#include "CEnemy.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;

class CPlayerBase;
class CInteractObject;
class CNavNode;

// ウォーロックの敵クラス
class CBoss : public CEnemy
{
public:
	// インスタンスのポインタを取得
	static CBoss* Instance();

	// コンストラクタ
	CBoss(std::vector<CVector> patrolPoints);
	// デストラクタ
	~CBoss();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 妖力の源の減少
	void PowerDown();

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
	// 死亡
	void Death() override;
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// ウォーロックのインスタンス
	static CBoss* spInstance;

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eIdle,		// 待機
		eWalk,		// 歩行
		eRun,		// 走行
		eAttack,	// 攻撃

		Num			// アニメーションの種類の数
	};

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		ePatrol,	// 巡回中
		eChase,		// プレイヤーを追いかける
		eLost,		// プレイヤーを見失う
		eAttack,	// プレイヤー攻撃
	};

	// 状態切り替え
	void ChangeState(int state) override;

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundTarget(CObjectBase* target) const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookTarget(CObjectBase* target) const;
	// プレイヤーを攻撃できるかどうか
	bool CanAttackPlayer() const;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 次に巡回するポイントを変更する
	void ChangePatrolPoint();

	// 待機状態の更新処理
	void UpdateIdle();
	// 巡回中の更新処理
	void UpdatePatrol();
	// 追いかける時の更新処理
	void UpdateChase();
	// プレイヤーを見失った時の更新処理
	void UpdateLost();
	// パンチ攻撃時の更新処理
	void UpdateAttack1();
	// 針攻撃時の更新処理
	void UpdateAttack2();
	// 仰け反り状態の更新処理
	void UpdateHit();
	// 死亡状態の更新処理
	void UpdateDeath();

	// 状態の文字列を取得
	std::string GetStateStr(EState state) const;
	// 状態の色を取得
	CColor GetStateColor(EState state) const;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示

	CNavNode* mpLostPlayerNode;	// プレイヤーを見失った位置のノード
	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// 次に巡回する番号

	std::vector<CNavNode*> mMoveRoute;	// 求めた最短経路記憶用
	int mNextMoveIndex;					// 次に移動するノードのインデックス値

	std::list<CObjectBase*> mTargets;	//標的のリスト

	bool mIsBattle;					// 戦闘状態か
	float mBattleIdletime;			// 戦闘時の待機時間
	CObjectBase* mpBattleTarget;	// 戦闘相手
	CCollider* mpAttack1Col;		// パンチ攻撃用のコライダー

	// 一番近くにある壊せるオブジェクトを取得
	CInteractObject* GetNearBreakObj() const;
	// 近くにある調べられるオブジェクトのリスト
	std::list<CInteractObject*> mNearBreakObjs;
	CCollider* mpSearchCol;	// 調べるオブジェクトを探知するコライダー

	// 自分とターゲットの間に壊せるオブジェクトがあるか
	bool IsBreakObject();
	// 攻撃力
	int mPower;
#if _DEBUG
	EScene mScene;
#endif

};