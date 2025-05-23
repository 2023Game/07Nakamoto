#pragma once
//キャラクタクラスのインクルード
#include "CPlayerBace.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;

/*
猫クラス
キャラクタクラスを継承
*/
class CCat : public CPlayerBace
{
public:
	//インスタンスのポインタの取得
	static CCat* Instance();

	// コンストラクタ
	CCat();
	// デストラクタ
	~CCat();

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

	// 猫のインスタンス
	static CCat* spInstance;

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

	// 猫の状態
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

};
