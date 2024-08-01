#include "CTaskManager.h"

//デフォルトコンストラクタ
CTaskManager::CTaskManager()
{
	mHead.mpNext = &mTail;
	mTail.mpPrev = &mHead;
}
//デストラクタ
CTaskManager::~CTaskManager()
{
	
}
//タスクの削除
void CTaskManager::Delete()
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		CTask* del = task;
		//次へ
		task = task->mpNext;
		//mEnabledがfalseなら削除
		if (del->mEnabled == false)
		{
			delete del;
		}
	}
}
//全インスタンス削除
void CTaskManager::AllDelete()
{
	//イテレータの生成
	std::vector<CCharacter*>::iterator itr;
	//イテレータを先頭へ
	itr = mpCharacters.begin();
	//最後まで繰り返し
	while (itr != mpCharacters.end())
	{
		//インスタンス削除
		delete* itr;
		itr = mpCharacters.erase(itr);
	}
}

//リストから削除
void CTaskManager::Remove(CTask* task)
{
	//タスクの前の次を、タスクの次にする
	task->mpPrev->mpNext = task->mpNext;
	//タスクの次の前を、タスクの前にする
	task->mpNext->mpPrev = task->mpPrev;
}
//更新
void CTaskManager::Update()
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		//更新処理を呼ぶ
		task->Update();
		//次へ
		task = task->mpNext;
	}
}
//描画
void CTaskManager::Render()
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext)
	{
		//描画処理を呼ぶ
		task->Render();
		//次へ
		task = task->mpNext;
	}
}
//リストに追加
void CTaskManager::Add(CTask* addTask)
{
	//mHeadの次から検索
	CTask* task = mHead.mpNext;

	//優先度の大きい順に挿入する
	//挿入位置の検索(優先度が同じか大きくなった前)
	//mPriority >= 0のこと
	while (addTask->mPriority < task->mPriority)
	{
		task = task->mpNext;
	}

	//mTailの前に追加
	//CTask* task = &mTail;
	//addTaskの次をtask
	addTask->mpNext = task;
	//addTaskの前をtaskの前に
	addTask->mpPrev = task->mpPrev;
	//addTaskの前の次をaddTaskに
	task->mpPrev->mpNext = addTask;
	//taskの前をaddTaskに
	task->mpPrev = addTask;
}

//衝突処理
void CTaskManager::Collision()
{
	//先頭から最後まで繰り返し
	CTask* task = mHead.mpNext;
	while (task->mpNext) {
		//衝突処理を呼ぶ
		task->Collision();
		//次へ
		task = task->mpNext;
	}
}

//タスクマネージャのインスタンス
CTaskManager* CTaskManager::mpInstance = nullptr;

//インスタンスの取得
CTaskManager* CTaskManager::GetInstance()
{
	//インスタンスが無ければ
	if (mpInstance == nullptr)
	{
		//インスタンスを生成
		mpInstance = new CTaskManager();
	}
	return mpInstance;
}
