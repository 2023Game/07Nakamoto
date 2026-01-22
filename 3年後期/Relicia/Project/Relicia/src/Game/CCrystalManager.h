#pragma once

class CCrystalObj;

class CCrystalMangage
{
public:
	// インスタンス
	static CCrystalMangage* Instance();
	// インスタンスの削除
	static void ClearInstance();

	// 生成しているアイテムリストを取得
	std::vector<CCrystalObj*>& GetItemObjs();
	// フィールドのアイテムの生成
	void SpawnItems();
	// アイテム管理リストに追加
	void AddItem(CCrystalObj* item);
	// アイテム管理リストから削除
	void RemoveItem(CCrystalObj* item);
	// 全てのアイテムを削除する
	void AllRemoveItems();

private:
	// インスタンス
	CCrystalMangage();
	// デストラクタ
	~CCrystalMangage();

};