#ifndef CCOLLIDERTRIANGLE_H
#define CCOLLIDERTRIANGLE_H

#include "CCollider.h"

/*
三角形コライダの定義
*/

class CColliderTriangle :public CCollider
{
public:
	//デフォルトコンストラクタ
	CColliderTriangle() {}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="v2">頂点3</param>
	CColliderTriangle(CCharacter* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);

	/// <summary>
	/// 三角コライダの設定
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	/// <param name="v2">頂点3</param>
	void SetColliderTriangle(CCharacter* parent, CMatrix* matrix,
		const CVector& v0, const CVector& v1, const CVector& v2);

	//描画
	void Render();

	//優先度の変更
	void ChangePriority();

};

#endif // !CCOLLIDERTRIANGLE_H

