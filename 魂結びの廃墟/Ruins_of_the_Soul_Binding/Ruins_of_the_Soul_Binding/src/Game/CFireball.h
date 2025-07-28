#pragma once
#include "CObjectBase.h"
#include "CModel.h"

// 呪符による火球のクラス
class CFireball : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ 
	/// </summary>
	/// <param name="speed">火球の速度</param>
	/// <param name="dist">火球が移動できる距離</param>
	CFireball(float speed, float dist);
	// デストラクタ
	~CFireball();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CModel* mpModel;		// 火球のモデルデータ
	CCollider* mpCollider;	// 火球のコライダー
	float mMoveSpeed;		// 移動速度
	float mMoveDist;		// 移動できる距離
	float mCurrDist;		// 現在の移動距離
};