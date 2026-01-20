#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"
#include <unordered_map>

class CGaugeUI3D;
class CDebugFieldOfView;
class CNavNode;

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float speed;		// アニメーションの再生速度（1.0で等倍）
	};

	// コンストラクタ
	CEnemy();
	// デストラクタ
	virtual ~CEnemy();

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 攻撃中か
	bool IsAttacking() const override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 後更新
	void LateUpdate() override;
	// 描画
	void Render() override;

protected:

	// 敵の状態
	enum class EState
	{
		eIdle,		// 待機状態
		eJoinNavGraph,	// 巡回状態に戻る
		ePatrol,	// 巡回中
		eChase,		// 追いかける
		eLost,		// 見失う
		eAttack,	// 攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡
	};

	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// 指定した位置まで移動する
	bool MoveTo(const CVector& targetPos, float speed);
	// 移動速度を取得
	virtual float GetMoveSpeed() const = 0;
	// カプセルコライダーの半径を取得
	virtual float GetBodyRadius() const = 0;

	// 指定したターゲットが視野範囲内に入ったかどうか
	bool IsFoundTarget(CObjectBase* target) const;
	// 現在位置からターゲットが見えているかどうか
	bool IsLookTarget(CObjectBase* target) const;
	// プレイヤーを攻撃するか確認
	bool CheckAttackPlayer();

	// 状態切り替え
	virtual void ChangeState(EState state);

	// 待機状態の更新処理
	virtual void UpdateIdle();
	// 最寄りのノードに移動
	virtual void UpdateJoinNavGraph();
	// 巡回中の更新処理
	virtual void UpdatePatrol();
	// 追いかける時の更新処理
	virtual void UpdateChase();
	// プレイヤーを見失ったときの更新処理
	virtual void UpdateLost();
	// 攻撃時の更新処理
	virtual void UpdateAttack(int index);
	// 仰け反り状態の更新処理
	virtual void UpdateHit();
	// 死亡状態の更新処理
	virtual void UpdateDeath();

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	EState mState;			// 状態
	int mStateStep;			// 状態内のステップ管理用
	float mElapsedTime;		// 経過時間計測用

	// 今使っている攻撃の種類
	int mAttackIndex;

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// 前後左右の移動速度
	float mMoveSpeedY;		// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;		// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	CGaugeUI3D* mpHpGauge;	// HPゲージ
	CVector mGaugeOffsetPos;// ゲージのオフセット座標

	bool mIsBattle;					// 戦闘状態か
	float mBattleIdletime;			// 戦闘時の待機時間
	CObjectBase* mpBattleTarget;	// 戦闘相手

	CNavNode* mpLostPlayerNode;	// プレイヤーを見失った位置のノード
	float mLostElapsedTime;		// 見失ってからの経過時間

	// 最寄りのノード保存用
	CNavNode* mpNearNode;
	// 巡回ノード保存用
	CNavNode* mpCurrentNode;
	// 
	std::unordered_map<CNavNode*, int> mVisitCount;

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離

#if _DEBUG
	CColor GetStateColor(EState state) const;

	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示
#endif
};

#endif
