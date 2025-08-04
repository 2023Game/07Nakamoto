#pragma once
//キャラクタクラスのインクルード
#include "CPlayerBase.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;
class CNavNode;
class CNavManager;
class CGaugeUI;

/*
猫クラス
キャラクタクラスを継承
*/
class CCat : public CPlayerBase
{
public:
	//インスタンスのポインタの取得
	static CCat* Instance();

	// コンストラクタ
	CCat();
	// デストラクタ
	~CCat();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	/// <summary>
	/// 操作するかどうか設定
	/// </summary>
	/// <param name="operate">trueであれば、操作開始</param>
	void SetOperate(bool operate) override;

	// プレイヤーのバウンディングボックスを返す
	const CBounds& GetBounds() const override;

private:
	CVector mFollowPos;	// 猫がプレイヤーについていく時の座標
	CVector mLookAtPos;	// 注視する座標

	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec() const;

	// 待機状態
	void UpdateIdle();
	// 追従状態
	void UpdateTracking();
	// 攻撃状態
	void UpdateAttack();
	// 死亡処理
	void UpdateDeath();

	// 追従時の移動経路を計算するかどうか
	bool IsCalcTrackingRoute() const;
	
	// 移動の更新処理
	void UpdateMove();

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		//eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eAttack,	// 斬り攻撃
		eKick,		// 蹴り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eHit,		// 仰け反り

		Num
	};

	// 猫のインスタンス
	static CCat* spInstance;

	// 猫の状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack1,	// ひっかき攻撃
		eAttack2,	// 攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eHit,		// 仰け反り
		eTracking,	// 追従
		eLost,		// プレイヤーを見失う
		eDeath,		// 死亡
	};
	// 状態を切り替え
	void ChangeState(int state) override;

	CCollider* mpAttackCol;

	CGaugeUI* mpHpGauge;	// HPゲージ

	float mMaxSt;	// スタミナの最大値
	float mSt;	// スタミナ
	bool mIsDash;	// ダッシュフラグ
	CGaugeUI* mpStGauge;	// スタミナゲージ

	// 現在位置からプレイヤーが見えているかどうか
	bool IsLookTarget(CObjectBase* target) const;

	// 追従するためのノード
	CNavNode* mpTrackingNode;
	// 追従ポイントのリスト
	std::vector<CNavNode*> mTrackingRouteNodes;
	// 次に巡回する番号
	int mNextTrackingIndex;
};
