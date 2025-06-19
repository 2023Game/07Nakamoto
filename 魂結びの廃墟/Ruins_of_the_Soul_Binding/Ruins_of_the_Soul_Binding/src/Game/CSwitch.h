#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CCollider.h"

class CSwitch : public CObjectBase
{
public:
	// コンストラクタ
	CSwitch(const CVector& pos);
	// デストラクタ
	~CSwitch();

	// ボタンが押されているかどうか
	bool IsSwitchOn();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update();
	// 描画
	void Render();

private:
	CModel* mpButtonModel;
	CCollider* mpButtonColMesh;

	bool mSwitch;
	// 初期の高さ
	CVector mDefaultPos;
	// 押された時の座標
	CVector mOffSetPos;
};