#ifndef CCOLLIDERLINE_H
#define CCOLLIDERLINE_H

#include "CCollider.h"

/*
線分コライダの定義
*/

class CColliderLine :public CCollider
{
public:

	enum class ESType
	{
		ELine1,
		ELine2,
		ELine3,
	};

	//デフォルトコンストラクタ
	CColliderLine() {}
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	CColliderLine(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);

	/// <summary>
	/// 線分コライダの設定
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="v0">頂点1</param>
	/// <param name="v1">頂点2</param>
	void ColliderLineSet(CCharacter* parent, CMatrix* matrix, const CVector& v0, const CVector& v1);

	//描画
	void Render();

	//優先度の変更
	void ChangePriority();
};

#endif // !CCOLLIDERLINE_H

