#include "CNavManager.h"
#include "CNavNode.h"
#include "CInput.h"
#include "CField.h"
#include <assert.h>

CNavManager* CNavManager::spInstance = nullptr;

// 経路管理クラスのインスタンスを取得
CNavManager* CNavManager::Instance()
{
	return spInstance;
}

// コンストラクタ
CNavManager::CNavManager()
	: CTask(ETaskPriority::eNavigation, 0, ETaskPauseType::eGame)
	, mIsRender(false)
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// デストラクタ
CNavManager::~CNavManager()
{
	spInstance = nullptr;

	auto itr = mNodes.begin();
	auto end = mNodes.end();
	while (itr != end)
	{
		CNavNode* del = *itr;
		itr = mNodes.erase(itr);
		delete del;
	}
}

// 経路単サック用のノードを追加
void CNavManager::AddNode(CNavNode* node)
{
	mNodes.push_back(node);
}

// 経路探索用のノードを取り除く
void CNavManager::RemoveNode(CNavNode* node)
{
	mNodes.remove(node);
}

///	指定したノードに接続できるノードを検索して設定
int CNavManager::FindConnectNavNodes(CNavNode* node, float distance)
{
	// 現在の接続先の情報を全てクリアしておく
	node->ClearConnects();

	for (CNavNode* findNode : mNodes)
	{
		//自分自身であれば、スルー
		if (findNode == node) continue;

		// 目的地専用ノードを考慮しない
		if (!node->mIsDestNode)
		{
			// 指定された距離の限界値を超える場合は、スルー
			float dist = (findNode->GetPos() - node->GetPos()).Length();
			if (dist > distance) continue;
		}
		
		CVector start = node->GetOffsetPos();
		CVector end = findNode->GetOffsetPos();
		CHitInfo hit;
		if (CField::Instance()->CollisionRay(start, end, &hit))
		{
			// 何かにヒットした場合は、遮蔽物があるので接続できない
			continue;
		}

		// 両方の条件を満たしたノードを接続リストに追加
		node->AddConnect(findNode);

	}
	return node->mConnectData.size();
}

// 全てのノードと経路を描画
void CNavManager::Render()
{
	// [SPACE]キーで経路探索ノードの描画モードを切り替え
	if (CInput::PushKey(VK_SPACE))
	{
		mIsRender = !mIsRender;
	}

	// 経路探索ノードを描画しないならば、移行処理しない
	if (!mIsRender) return;

	// リスト内のノード全て描画
	for (CNavNode* node : mNodes)
	{
		node->Render();
	}
}
