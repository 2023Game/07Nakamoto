#pragma once
//キャラクタクラスのインクルード
#include "CPlayerBase.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;
class CDebugFieldOfView;
class CGaugeUI;
class CDemonPower;
class CHandGlow;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer2 : public CPlayerBase
{
public:
	//インスタンスのポインタの取得
	static CPlayer2* Instance();

	// コンストラクタ
	CPlayer2();
	// デストラクタ
	~CPlayer2();

	// 更新
	void Update() override;

	// ステータスを整数にして取得する
	int GetState();

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

	// 追従する位置を設定
	void SetTrail();
	// 追従用の配列を取得
	const std::vector<CVector>& GetTrail() const;

	// 行動の状態を変更
	void ChangeAction();
	// 一緒に行動しているか
	bool GatActingTogether();

private:
	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec();

	// 待機状態
	void UpdateIdle();
	// 仰け反り
	void UpdateHit();
	// 死亡処理
	void UpdateDeath();
	// 妖力を注いでいる
	void UpdateChanneling();

	// 移動の更新処理
	void UpdateMove();

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eIdle,		// 待機
		eWalk,		// 歩行
		eRun,		// 走行
		eHit,		// 仰け反り
		eDeath,		// 死亡

		eChannelingStart,	// 妖力を流し込み開始
		eChanneling,		// 妖力を流し込み中
		eChannelingEnd,	// 妖力を流し込み終了

		//eAttack,	// 攻撃

		Num
	};

	// プレイヤーのインスタンス
	static CPlayer2* spInstance;

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		eHit,		// 仰け反り
		eDeath,		// 死亡
		eChanneling	// 妖力を注いでいる
	};
	// 状態を切り替え
	void ChangeState(int state) override;

	CGaugeUI* mpHpGauge;	// HPゲージ

	int mMaxSt;	// スタミナの最大値
	int mSt;	// スタミナ
	CGaugeUI* mpStGauge;	// スタミナゲージ

	bool mTogether;	// 猫と一緒にいるか

	// 妖力を流し込んでいる妖力の源のオブジェクト
	CDemonPower* mpChannelingDemonPower;
	float mChannelingTime;	// 妖力を流し込んでいる時間

	// 手の光のエフェクト
	//CHandGlow* mpHandGlow;

	// 追従用の移動履歴を保管
	std::vector<CVector> mTrails;
	// 最後に位置を保存したときのプレイヤーの位置
	CVector mLastPos;

#if _DEBUG
	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示
#endif
};
