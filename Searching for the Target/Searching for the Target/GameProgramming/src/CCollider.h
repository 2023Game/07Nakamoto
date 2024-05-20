#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "CCharacter.h"

class CCollisionManager;

/*
コライダクラス
衝突判定データ
*/

class CCollider : public CTransform , public CTask {
	friend CCollisionManager;
public:
	//デストラクタ
	~CCollider();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="parent">親</param>
	/// <param name="matrix">親行列</param>
	/// <param name="position">位置</param>
	/// <param name="radius">半径</param>
	CCollider(CCharacter* parent, CMatrix* matrix,
		const CVector& position, float radius);

	//親ポインタの取得
	CCharacter* GetParent();
	//描画
	void Render();

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	/// <returns>true(衝突している) false(衝突していない)</returns>
	static bool Collision(CCollider* m, CCollider* o);

protected:
	CCharacter* mpParent;	//親
	CMatrix* mpMatrix;		//親行列
	float mRadius;			//半径
};

#endif // !CCOLLIDER_H
