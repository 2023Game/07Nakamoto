#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H

#include "CTaskManager.h"

class CCollisionManager :public CTaskManager
{
public:
	//インスタンスの取得
	static CCollisionManager* GetInstance();
	//衝突処理
	void Collison();
private:
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
};

#endif // !CCOLLISIONMANAGER_H

