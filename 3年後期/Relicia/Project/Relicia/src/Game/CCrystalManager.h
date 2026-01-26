#pragma once

class CCrystalObj;

class CCrystalManager
{
public:
	// インスタンス
	static CCrystalManager* Instance();
	// インスタンスの削除
	static void ClearInstance();

	// 生成しているクリスタルリストを取得
	std::vector<CCrystalObj*>& GetItemObjs();
	// フィールドのクリスタルの生成
	void SpawnCrystals();
	// クリスタル管理リストに追加
	void AddCrystal(CCrystalObj* item);
	// クリスタル管理リストから削除
	void RemoveCrystal(CCrystalObj* item);
	// 全てのクリスタルを削除する
	void AllRemoveCrystals();
	// インスタンスを破棄する
	void Destroy();

private:
	// インスタンス
	CCrystalManager();
	// デストラクタ
	~CCrystalManager();

	static CCrystalManager* spInstance;	// インスタンス
	std::vector<CCrystalObj*> mpCrystalObjs;	// 配置中のクリスタルのリスト
};