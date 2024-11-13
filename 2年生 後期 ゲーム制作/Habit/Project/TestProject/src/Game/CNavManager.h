#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CNavNode;

// 経路探索処理の管理クラス
class CNavManager : public CTask
{
public:
	// 経路管理クラスのインスタンスを取得
	static CNavManager* Instance();

	// コンストラクタ
	CNavManager();
	// デストラクタ
	~CNavManager();

	// 経路単サック用のノードを追加
	void AddNode(CNavNode* node);
	// 経路探索用のノードを取り除く
	void RemoveNode(CNavNode* node);

	/// <summary>
	///	指定したノードに接続できるノードを検索して設定
	/// (間に遮蔽物があるノードには接続できない)
	/// </summary>
	/// <param name="node">接続できるノードを検索するノード</param>
	/// <param name="distance">接続できる距離の限界値</param>
	/// <returns>接続したノードの数</returns>
	int FindConnectNavNodes(CNavNode* node, float distance);

	// 全てのノードと経路を描画
	void Render() override;

private:
	// 経路管理クラスのインスタンスへのポインタ
	static CNavManager* spInstance;

	// 経路探索用のノードのリスト
	std::list<CNavNode*> mNodes;

	// 経路探索のデバッグ表示を行うかどうか
	bool mIsRender;
};