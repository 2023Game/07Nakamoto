#pragma once
#include "CEnemy.h"

// 幽霊の敵クラス
class CGhost : public CEnemy
{
public:
	// コンストラクタ
	CGhost();
	// デストラクタ
	~CGhost();

	// 攻撃中か
	bool IsAttacking() const override;
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

		eIdle,		// 待機

		Num
	};

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eChase,		// 追いかける
	};

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 状態切り替え
	void ChangeState(int state) override;

	// 待機状態の更新処理
	void UpdateIdle();
	// 追いかける時の更新処理
	void UpdateChase();

	CObjectBase* mpBattleTarget;	// 戦闘相手

};