#pragma once
#include "CEnemy.h"

// 視野範囲のデバッグ表示クラスの前宣言
class CDebugFieldOfView;

class CNavNode;

// 幽霊の敵クラス
class CGhost : public CEnemy
{
public:
	// コンストラクタ
	CGhost(std::vector<CVector> patrolPoints);
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

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

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
		ePatrol,	// 巡回中
		eChase,		// 追いかける
	};

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 状態切り替え
	void ChangeState(int state) override;

	// プレイヤーが視野範囲内に入ったかどうか
	bool IsFoundPlayer() const;
	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookPlayer() const;

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 次に巡回するポイントを変更する
	void ChangePatrolPoint();

	// 待機状態の更新処理
	void UpdateIdle();
	// 徘徊状態の更新処理
	void UpdateePatrol();
	// 追いかける時の更新処理
	void UpdateChase();

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離
	//CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示

	// 巡回ポイントのリスト
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// 次に巡回する番号

	std::vector<CNavNode*> mMoveRoute;	// 求めた最短経路記憶用
	int mNextMoveIndex;					// 次に移動するノードのインデックス値

};