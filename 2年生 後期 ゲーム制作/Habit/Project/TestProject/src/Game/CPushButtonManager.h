#pragma once
#include "CTask.h"
#include <list>

// スイッチのタスク管理クラス
class CPushButtonManager
{
public:
	// インスタンスを取得
	static CPushButtonManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	/// <summary>
	/// 指定したボタンをリストに追加
	/// </summary>
	/// <param name="pushbottun">追加するボタン</param>
	void Add(CTask* pushbottun);
	/// <summary>
	/// 指定したタスクをリストから取り除く
	/// </summary>
	/// <param name="pushbottun">取り除くボタン</param>
	void Remove(CTask* pushbottun);

	// ボタンのリストを取得
	//const std::list<CTask*>& GetTasks() const;

private:
	// コンストラクタ
	CPushButtonManager();
	// デストラクタ
	~CPushButtonManager();

	// タスクマネージャのインスタンス
	static CPushButtonManager* mpInstance;

	//ボタンのリスト
	std::list<CTask*> mPushButtonList;

};
