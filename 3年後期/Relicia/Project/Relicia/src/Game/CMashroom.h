#pragma once
#include "CEnemy.h"

class CColliderSphere;
class CNavNode;

// キノコの敵クラス
class CMashroom : public CEnemy
{
public:
	// コンストラクタ
	CMashroom();
	// デストラクタ
	~CMashroom();

	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// 属性ダメージを受ける
	void TakeDamage(int damage, ElementType type, CObjectBase* causer) override;

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
		eAttack1,	// 頭突き攻撃
		eAttack2,	// 回転攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡
		
		Num
	};

	// 攻撃の種類
	enum class EAttackID
	{
		None = -1,

		eHeadbutt,	// 頭突き
		eSpin,		// スピン攻撃

		Num
	};

	// 移動速度を取得
	float GetMoveSpeed() const override;
	// カプセルコライダーの半径を取得
	float GetBodyRadius() const override;

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
	// 頭突き攻撃時の更新処理
	void UpdateHeadbutt();
	// スピン攻撃時の更新処理
	void UpdateSpin();
	// 仰け反り状態の更新処理
	void UpdateHit() override;
	// 死亡状態の更新処理
	void UpdateDeath() override;

	CCollider* mpAttack1Col;		// 攻撃用のコライダー
};