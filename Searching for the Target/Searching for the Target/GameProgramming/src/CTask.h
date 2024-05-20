#ifndef CTASK_H
#define CTASK_H

class CTaskManager;
class CCollisionManager;

/*
タスククラス
タスクリストの要素
*/

class CTask {
	friend CTaskManager;
	friend CCollisionManager;
public:
	//デフォルトコンストラクタ
	CTask()
		:mpNext(nullptr),mpPrev(nullptr),mPriority(0),mEnabled(true)
	{}
	//デストラクタ
	virtual ~CTask() {}
	//更新
	virtual void Update() {}
	//描画
	virtual void Render() {}
protected:
	int mPriority;	//優先順位
	bool mEnabled;	//有効フラグ
private:
	CTask* mpNext;	//次ポインタ
	CTask* mpPrev;	//前ポインタ
};

#endif // !CTASK_H
