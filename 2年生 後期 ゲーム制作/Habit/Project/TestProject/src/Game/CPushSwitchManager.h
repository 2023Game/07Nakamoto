#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CPushSwitch;

// スイッチのタスク管理クラス
class CPushSwitchManager : public CTask
{
public:
	// インスタンスを取得
	static CPushSwitchManager* Instance();

	// コンストラクタ
	CPushSwitchManager();
	// デストラクタ
	~CPushSwitchManager();

	/// <summary>
	/// 指定したスイッチをリストに追加
	/// </summary>
	/// <param name="pushswitch">追加するスイッチ</param>
	void AddSwitch(CPushSwitch* pushswitch);
	/// <summary>
	/// 指定したスイッチをリストから取り除く
	/// </summary>
	/// <param name="pushswitch">取り除くスイッチ</param>
	void RemoveSwitch(CPushSwitch* pushswitch);


private:
	// スイッチ管理クラスのインスタンスへのポインタ
	static CPushSwitchManager* spInstance;

	//スイッチのリスト
	std::vector<CPushSwitch*> mPushSwitchList;

};
