#pragma once
#include "CEnemy.h"

// サボテンの敵クラス
class CCactus : public CEnemy
{
public:
	// コンストラクタ
	CCactus();
	// デストラクタ
	~CCactus();

	// 属性ダメージを受ける
	void TakeDamage(int damage, ElementType type, CObjectBase* causer) override;

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eIdleBattle,// 戦闘中の待機
		eWalk,		// 歩行
		eAttack1,	// パンチ攻撃
		eAttack2,	// 針攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡

		Num
	};

	// サボテンの敵の状態
	//enum class EState
	//{
	//	eIdle,		// 待機状態
	//	ePatrol,	// 巡回中
	//	eChase,		// 追いかける
	//	eLost,		// 見失う
	//	eAttack1,	// パンチ攻撃
	//	eAttack2,	// 針攻撃
	//	eHit,		// 仰け反り
	//	eDeath,		// 死亡
	//};

	// 攻撃の種類
	enum class EAttackID
	{
		None = -1,

		ePunch,		// パンチ攻撃
		eNeedle,	// 棘攻撃

		Num
	};

	// 移動速度を取得
	float GetMoveSpeed() const override;
	// カプセルコライダーの半径を取得
	float GetBodyRadius() const override;

	// 針を発射
	void ShotNeedle();

	// 状態切り替え
	void ChangeState(EState state) override;

	// 待機状態の更新処理
	void UpdateIdle() override;
	// 最寄りのノードに移動
	void UpdateJoinNavGraph() override;
	// 巡回中の更新処理
	void UpdatePatrol() override;
	// 追いかける時の更新処理
	void UpdateChase() override;
	// 攻撃時の更新処理
	void UpdateAttack(int index) override;
	// パンチ攻撃時の更新処理
	void UpdatePunch();
	// 針攻撃時の更新処理
	void UpdateNeedle();
	// 仰け反り状態の更新処理
	void UpdateHit() override;
	// 死亡状態の更新処理
	void UpdateDeath() override;

	//bool mIsBattle;					// 戦闘状態か
	//float mBattleIdletime;			// 戦闘時の待機時間
	//CObjectBase* mpBattleTarget;	// 戦闘相手
	CCollider* mpAttack1Col;		// パンチ攻撃用のコライダー
	int mShotNeedleCount;			// 針を発射した回数
};
