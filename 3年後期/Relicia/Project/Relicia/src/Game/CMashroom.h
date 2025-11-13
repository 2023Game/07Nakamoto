#pragma once
#include "CEnemy.h"

// 蝙蝠の敵クラス
class CMashroom : public CEnemy
{
public:
	// コンストラクタ
	CMashroom();
	// デストラクタ
	~CMashroom();

	// 更新
	void Update() override;

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eAttack1,	// 頭突き攻撃

		Num
	};

	// 状態
	enum class EState
	{
		eIdle,		// 待機状態
		eAttack1,	// 頭突き攻撃
	};

	// 状態切り替え
	//void ChangeState(int state) override;

};