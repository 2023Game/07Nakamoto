#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CCollider.h"

class CPressAndHoldSwitch : public CObjectBase
{
	// コンストラクタ
	CPressAndHoldSwitch(const CVector& pos);
	// デストラクタ
	~CPressAndHoldSwitch();

	// ボタンが押されているかどうか
	bool IsSwitchOn();

	// ボタンのオンオフ切り替え
	void ChangeSwith();

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
	void Render() override;

private:
	CModel* mpButtonModel;
	CCollider* mpButtonColMesh;

	CObjectBase* mpLastPushedObject;
	CObjectBase* mpPushedObject;

	bool mSwitch;
	// 初期の高さ
	CVector mDefaultPos;
	// 押された時の座標
	CVector mOffSetPos;

	// 経過時間計測用
	float mElapsedTime;
};