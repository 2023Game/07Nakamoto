#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CNavNode;

// 経路探索処理の管理クラス
class CNavManager : public CTask
{
public:

private:
	// 経路管理クラスのインスタンスへのポインタ
	static CNavManager* spInstance;

	// 経路探索用のノードのリスト
	std::list<CNavNode*> mNodes;

	// 経路探索のデバッグ表示を行うかどうか
	bool mIsRender;
};