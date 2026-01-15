#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

class CGaugeUI3D;

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
		ePatrol,	// 巡回中
		eChase,		// 追いかける
		eLost,		// 見失う
		eAttack,	// 攻撃
		eHit,		// 仰け反り
		eDeath,		// 死亡
	};

	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// 状態切り替え
	virtual void ChangeState(EState state);

	// 待機状態の更新処理
	virtual void UpdateIdle();
	// 巡回中の更新処理
	virtual void UpdatePatrol();
	// 追いかける時の更新処理
	virtual void UpdateChase();
	// プレイヤーを見失ったときの更新処理
	virtual void UpdateLost();
	// 攻撃時の更新処理
	virtual void UpdateAttack();
	// 仰け反り状態の更新処理
	virtual void UpdateHit();
	// 死亡状態の更新処理
	virtual void UpdateDeath();

	// 今使っている攻撃の種類
	int mAttackIndex;

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);


	EState mState;			// 状態
	int mStateStep;			// 状態内のステップ管理用
	float mElapsedTime;		// 経過時間計測用

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// 前後左右の移動速度
	float mMoveSpeedY;		// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;		// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	CGaugeUI3D* mpHpGauge;	// HPゲージ
	CVector mGaugeOffsetPos;// ゲージのオフセット座標
};

#endif
