#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer2 : public CXCharacter
{
public:
	//インスタンスのポインタの取得
	static CPlayer2* Instance();

	// コンストラクタ
	CPlayer2();
	// デストラクタ
	~CPlayer2();

	// 更新
	void Update();

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

	// 描画
	void Render();

private:
	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec() const;

	// 待機状態
	void UpdateIdle();
	// 斬り攻撃
	void UpdateAttack1();
	// 蹴り攻撃
	void UpdateAttack2();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 仰け反り
	void UpdateHit();

	// 移動の更新処理
	void UpdateMove();

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
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type, bool restart = false);

	// プレイヤーのインスタンス
	static CPlayer2* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float speed;		// アニメーション速度（1.0で等倍）
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		eAttack1,	// 斬り攻撃
		eAttack2,	// 蹴り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eHit,		// 仰け反り
	};
	// 状態を切り替え
	void ChangeState(EState state);

	EState mState;				// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

	CVector mMoveSpeed;			// 前後左右の移動速度
	float mMoveSpeedY;			// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;			// 接地しているかどうか
	CVector mGroundNormal;		// 接地している地面の法線

	CCollider* mpBodyCol;		// 本体のコライダー

	CTransform* mpRideObject;
};
