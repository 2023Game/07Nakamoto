#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

class CPlayer2;

class CInventory : public CTask
{
public:
	// コンストラクタ
	CInventory();
	// デストラクタ
	~CInventory();

	// 開く
	void Open();
	// 閉じる
	void Close();
	// インベントリが開いているかどうか
	bool IsOpened() const;

	// 決める
	void Decide(int select);
	// アイテムを追加する
	void AddItem();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CImage* mpBackground;
	CImage* mpInventoryFrame;
	CImage* mpBackMenu;
	CImage* mpSelectFrame;
	CImage* mpChoco;

	std::vector<CImage*> mItemList;

	int mSelectIndex;
	bool mIsOpened;
};