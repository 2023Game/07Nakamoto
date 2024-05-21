#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H

#include "CTask.h"

/*
タスクマネージャ
タスクリストの管理
*/
class CTaskManager {
public:
	//デストラクタ
	virtual ~CTaskManager();

	//タスクの削除
	void Delete();

	/// <summary>
	/// リストから削除
	/// </summary>
	/// <param name="task">タスクのポインタ</param>
	void Remove(CTask* task);

	//更新
	void Update();
	//描画
	void Render();

	/// <summary>
	/// リストに追加
	/// </summary>
	/// <param name="addTask">タスクのポインタ</param>
	void Add(CTask* addTask);

	//インスタンスの取得
	static CTaskManager* GetInstance();

protected:
	//デフォルトコンストラクタ
	CTaskManager();

	CTask mHead;	//先頭タスク
	CTask mTail;	//最終タスク

private:
	//タスクマネージャのインスタンス
	static CTaskManager* mpInstance;
};

#endif // !CTASKMANAGER_H
