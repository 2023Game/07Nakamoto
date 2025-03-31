#pragma once

class CInteractUI;
class CInteractObject;

class CGameUI
{
public:
	// インスタンスを取得する
	static CGameUI* Instance();
	// インスタンスを破棄する
	static void ClearInstance();

	// コンストラクタ
	CGameUI();
	// デストラクタ
	~CGameUI();

	/// <summary>
	/// 指定したオブジェクトの調べるUIを表示
	/// </summary>
	/// <param name="obj">UIを表示する調べるオブジェクト</param>
	void ShowInteractUI(CInteractObject* obj);
	// 調べるUIを非表示にする
	void HideInteractUI();

private:
	static CGameUI* spInstance;	// CGameUIのインスタンス
	CInteractUI* mpInteractUI;	// 調べるUI
};