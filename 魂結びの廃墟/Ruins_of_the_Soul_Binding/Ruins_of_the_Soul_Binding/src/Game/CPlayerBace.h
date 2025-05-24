#pragma once
#include "CXCharacter.h"

class CCollider;

class CPlayerBace : public CXCharacter
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
	CPlayerBace();
	// デストラクタ
	~CPlayerBace();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render();

protected:
	// プレイヤーの初期化
	void InitPlayer(std::string path, const std::vector<AnimData>* pAnimData);

	// 状態切り替え
	virtual void ChangeState(int state);

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false);

	int mState;				// 状態
	int mStateStep;			// 状態内のステップ管理用
	float mElapsedTime;		// 経過時間計測用

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// 前後左右の移動速度
	float mMoveSpeedY;		// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;		// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	CTransform* mpRideObject;

};	