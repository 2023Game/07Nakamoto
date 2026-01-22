#include "CCrystalManager.h"

// インスタンス
CCrystalMangage* CCrystalMangage::Instance()
{
	return nullptr;
}

// インスタンス
CCrystalMangage::CCrystalMangage()
{
}

// デストラクタ
CCrystalMangage::~CCrystalMangage()
{
}

// インスタンスの削除
void CCrystalMangage::ClearInstance()
{
}

// 生成しているアイテムリストを取得
std::vector<CCrystalObj*>& CCrystalMangage::GetItemObjs()
{
	// TODO: return ステートメントをここに挿入します
}

// フィールドのアイテムの生成
void CCrystalMangage::SpawnItems()
{
}

// アイテム管理リストに追加
void CCrystalMangage::AddItem(CCrystalObj* item)
{
}

// アイテム管理リストから削除
void CCrystalMangage::RemoveItem(CCrystalObj* item)
{
}

// 全てのアイテムを削除する
void CCrystalMangage::AllRemoveItems()
{
}

