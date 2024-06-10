#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

/*
ターゲットクラス
キャラクタクラスを継承
*/
class CTarget :public CCharacter {
public:
	//Targetのタイプ
	enum class EState
	{
		ESTAY,		//滞在
		EMOVE1,		//横移動
	};

	CTarget(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale, EState state);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	void Collision(CCollider* m, CCollider* o);

	//衝突処理
	void Collision();

	//更新処理
	void Update();

private:
	//タイプ
	EState mType;
	//コライダ
	CCollider mCollider;

	int count;
};

#endif // !CTARGET_H
