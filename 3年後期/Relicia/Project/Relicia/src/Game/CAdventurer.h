#pragma once
#include "CXCharacter.h"
#include "CSound.h"
#include "CrystalData.h"

class CCollider;
class CSword;
class CPlayerHpUI;
class CElementSlotUI2;
class CNavNode;
struct ItemData;

class CAdventurer : public CXCharacter
{
public:
	// インスタンスのポインタの取得
	static CAdventurer* Instance();

	// コンストラクタ
	CAdventurer();
	// デストラクタ
	~CAdventurer();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;

	// 装備したスロット番号の属性を装備する
	//void EquipElement(int slotIndex);

	// 経路探索用のノード取得
	CNavNode* GetNavNode() const;

	// 更新
	void Update() override;
	// 後更新
	void LateUpdate() override;
	// 描画
	void Render() override;

private:
	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec() const;

	// 待機状態
	void UpdateIdle();
	// 斬り攻撃
	void UpdateAttack();
	// 回転切り
	void UpdateAttack2();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 仰け反り
	void UpdateHit();
	// 死亡
	void UpdateDeath();

	// 移動の更新処理
	void UpdateMove();

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eRun,		// 走行
		eAttack,	// 斬り攻撃
		eAttack2,	// 回転切り
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eHit,		// 仰け反り
		eDeath,		// 死亡

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type, bool restart = false);

	// プレイヤーのインスタンス
	static CAdventurer* spInstance;

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
		eAttack,	// 斬り攻撃
		eAttack2,	// 回転切り
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eHit,		// 仰け反り
		eDeath,		// 死亡
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

	CSound* mpSlashSE;			// 剣のSE
	bool mIsPlayedSlashSE;		// 剣の攻撃SEのフラグ
	bool mIsSpawnedSlashEffect;	// 斬撃SEのフラグ

	CPlayerHpUI* mpHpGauge;	// プレイヤーのUI

	// 剣のクラス
	CSword* mpSword;

	// 属性スロットの装備枠
	CElementSlotUI2* mpElementEquipment;

	CNavNode* mpNavNode;// 経路探索用のノードポインタ

};