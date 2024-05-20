#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"

/*
ターゲットクラス
キャラクタクラスを継承
*/
class CTarget :public CCharacter {
public:
	CTarget(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="m">コライダ1</param>
	/// <param name="o">コライダ2</param>
	void Collision(CCollider* m, CCollider* o);

	//更新処理
	void Update();

private:
	//コライダ
	CCollider mCollider;

	int count;
};

#endif // !CTARGET_H
