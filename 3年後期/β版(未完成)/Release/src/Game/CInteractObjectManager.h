#pragma once
#include <vector>

class CInteractObject;

// 調べるオブジェクトの管理用クラス
class CInteractObjectManager
{
public:
	// インスタンスを取得
	static CInteractObjectManager* Instance();
	// インスタンスを破棄
	static void CIearInstance();

	// コンストラクタ
	CInteractObjectManager();
	// デストラクタ
	~CInteractObjectManager();

	// 調べるオブジェクトを追加
	void Add(CInteractObject* obj);
	// 調べるオブジェクトを取り除く
	void Remove(CInteractObject* obj);

	/// <summary>
	/// 指定した座標に近い調べるオブジェクトを返す
	/// </summary>
	/// <param name="pos">調べる座標</param>
	/// <param name="distance">調べる距離</param>
	/// <param name="list">見つかったオブジェクトを格納するリスト</param>
	/// <returns>trueであれば、近くのオブジェクトが存在した</returns>
	bool GetNearInteractObjects(const CVector& pos, float distance,
		std::vector<CInteractObject*>& list);

private:
	// インスタンス
	static CInteractObjectManager* spInstance;
	// 生成済みの調べるオブジェクトのリスト
	std::vector<CInteractObject*> mInteractObjects;
};