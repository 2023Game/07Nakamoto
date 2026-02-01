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

	// 持っているノードの初期化
	void InitNav();
	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	// 攻撃中か
	bool IsAttacking() const override;

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;
	// 死亡状態にする
	void Death() override;

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
		eAlert,		// 警戒
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

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 次に移動するノードを設定する
	CNavNode* SelectNextPatrolNode();

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
	// 警戒している時の処理
	virtual void UpdateAlert();
	// 攻撃時の更新処理
	virtual void UpdateAttack(int index);
	// 仰け反り状態の更新処理
	virtual void UpdateHit();
	// 死亡状態の更新処理
	virtual void UpdateDeath();

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	// 動かせるオブジェクトに当たっている時間
	void UpdateMoveObj(float delta);
	// 一定時間塞がれているか
	bool ShouldAttackBlockingObj() const;
	// 進路を塞いでいるオブジェクトを攻撃するか
	bool TryAttackBlockingObj();

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

	CNavNode* mpNearNode;		// 最寄りのノード保存用
	CNavNode* mpCurrentNode;	// 巡回ノード保存用
	CNavNode* mpNextNode;		// 次に移動するノード保存用
	CNavNode* mpPrevNode;		// 戻り防止用

	float mFovAngle;	// 視野範囲の角度
	float mFovLength;	// 視野範囲の距離

	float mMoveObjElapsed;	// 動かせるオブジェクトに当たっている時間
	CObjectBase* mpBlockingObj;	// 進路を阻んでいるオブジェクト保存用
	bool mIsBlockedThisFrame;	// 動かせるオブジェクトと当たっているか

	CVector mRote;

	CVector mLookForwad;	// 見渡す前の正面方向ベクトル
	CVector mLookStarDir;	// 見渡しアニメーションの開始時の方向ベクトル
	CVector mLookEndDir ;	// 見渡しアニメーションの終了時の方向ベクトル

#if _DEBUG
	CColor GetStateColor(EState state) const;

	CDebugFieldOfView* mpDebugFov;	// 視野範囲のデバッグ表示
#endif
};

#endif
