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
	//コライダタイプ
	enum class EType {
		ESPHERE,	//球コライダ
		ETRIANGLE,	//三角コライダ
		ELINE,		//線分コライダ
	};

	//デフォルトコンストラクタ
	CCollider();
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
	//コライダタイプの取得
	CCollider::EType GetType();

	//描画
	void Render();

	/// <summary>
	/// 斜面にプレイヤーを合わせる
	/// </summary>
	/// <param name="p">プレイヤー</param>
	/// <param name="t">三角コライダ</param>
	/// <returns></returns>
	static CVector Slope(CCollider* p, CCollider* t, CVector* adjust);

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	/// <returns>true(衝突している) false(衝突していない)</returns>
	static bool Collision(CCollider* m, CCollider* o);

	/// <summary>
	/// 三角形と球の衝突判定
	/// </summary>
	/// <param name="triangle">三角コライダ</param>
	/// <param name="sphere">球コライダ</param>
	/// <param name="adjust">調整値:衝突しない位置まで戻す値</param>
	/// <returns>true(衝突している) false(衝突していない)</returns>
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* adjust);

	/// <summary>
	/// 三角形と線分の衝突範囲
	/// </summary>
	/// <param name="triangle">三角コライダ</param>
	/// <param name="line">線分コライダ</param>
	/// <param name="adjust">調整値:衝突しない位置まで戻す値</param>
	/// <returns>true(衝突している) false(衝突していない)</returns>
	static bool CollisionTriangleLine(CCollider* triangle,
		CCollider* line, CVector* adjust);

	/// <summary>
	/// 優先度の変更
	/// </summary>
	/// <param name="priority">優先度</param>
	void ChangePriority(int priority);

	//優先度の変更
	virtual void ChangePriority();

protected:
	EType mType;	//コライダタイプ

	//頂点
	CVector mV[3];

	CCharacter* mpParent;	//親
	CMatrix* mpMatrix;		//親行列
	float mRadius;			//半径
};

#endif // !CCOLLIDER_H
