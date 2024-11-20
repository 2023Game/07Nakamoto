#pragma once
#include "CObjectBase.h"

// 敵が設置する罠
class CTrap : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pwner">罠を生成したオブジェクト</param>
	/// <param name="pos">生成位置</param>
	CTrap(CObjectBase* owner, const CVector& pos);
	// デストラクタ
	~CTrap();

	// 遮蔽物チェックや表示用に、少し上に上げたノードの座標を返す
	CVector GetOffsetPos() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;	// このエフェクトの持ち主
	CVector mPosition;
	CColor mColor;
};