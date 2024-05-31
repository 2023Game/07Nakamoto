#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"
#include "CCollider.h"

//衝突処理範囲より離れているコライダは衝突処理をしない
#define COLLISIONRANGE 100

class CCollisionManager :public CTaskManager
{
public:
	//インスタンスの取得
	static CCollisionManager* GetInstance();
	//衝突処理
	void Collison();

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">コライダ</param>
	/// <param name="range">範囲</param>
	void Collision(CCollider* c, int range);
private:
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
};

#endif // !CCOLLISIONMANAGER_H

