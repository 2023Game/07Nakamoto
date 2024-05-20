#include "CCollisionManager.h"
#include "CCollider.h"

//マネージャのインスタンス
CCollisionManager* CCollisionManager::mpInstance = nullptr;
//インスタンスの取得
CCollisionManager* CCollisionManager::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CCollisionManager();
	}
	return mpInstance;
}

//衝突処理
void CCollisionManager::Collison()
{
	//現在位置を先頭にする
	CCollider* task = (CCollider*)mHead.mpNext;
	//最後まで来たら終了する
	while (task->mpNext)
	{
		//現在位置の次を求める
		CCollider* next = (CCollider*)task->mpNext;
		//次が0になったら終了
		while (next->mpNext)
		{
			//親の衝突判定を呼び出す
			//自分の衝突処理
			if (task->mpParent)
				task->mpParent->Collision(task, next);
			//相手の衝突処理
			if (next->mpParent)
				next->mpParent->Collision(next, task);
			//次を求める
			next = (CCollider*)next->mpNext;
		}
		//現在位置を次にする
		task = (CCollider*)task->mpNext;
	}
}