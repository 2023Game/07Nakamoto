#pragma once
#include "CEnemy.h"

class CColliderSphere;

// 蝙蝠の敵クラス
class CMashroom : public CEnemy
{
public:
	// コンストラクタ
	CMashroom();
	// デストラクタ
	~CMashroom();

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
	// 死亡
	void Death() override;

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
		eDeash,		// 死亡
		
		Num
	};

	// 状態
	enum class EState
	{
		eIdle,		// 待機状態
		eAttack1,	// 頭突き攻撃

		eDeath,		// 死亡
	};

	// 状態切り替え
	void ChangeState(int state) override;

	// 死亡状態の更新処理
	void UpdateDeath();

};