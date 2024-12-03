#include "CPushButtonManager.h"

// タスクマネージャのインスタンス
CPushButtonManager* CPushButtonManager::mpInstance = nullptr;

CPushButtonManager* CPushButtonManager::Instance()
{
	// インスタンスが無ければ、新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new CPushButtonManager();
	}
	return mpInstance;
}

// インスタンスの破棄
void CPushButtonManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

// コンストラクタ
CPushButtonManager::CPushButtonManager()
{
}

// デストラクタ
CPushButtonManager::~CPushButtonManager()
{
	std::list<CTask*>& list = mPushButtonList;
	// 先頭から最後まで繰り返し
	auto itr = list.begin();
	auto end = list.end();
	while (itr != end)
	{
		// 削除タスクを記憶しておく
		CTask* del = *itr;
		itr = list.erase(itr);
		delete del;
	}
}

// 指定したボタンをリストに追加
void CPushButtonManager::Add(CTask* pushbutton)
{
	auto it = std::find(mPushButtonList.begin(), mPushButtonList.end(), pushbutton);
	if (it != mPushButtonList.end())return;

	// リストの最後尾に追加
	mPushButtonList.push_back(pushbutton);
}

// 指定したボタンをリストから取り除く
void CPushButtonManager::Remove(CTask* pushbutton)
{
	mPushButtonList.remove(pushbutton);
}

//// ボタンのリストを取得
//const std::list<CTask*>& CPushButtonManager::GetTasks() const
//{
//	return mPushButtonList;
//}